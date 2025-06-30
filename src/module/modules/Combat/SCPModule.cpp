#include "SCPModule.hpp"
#include "IL2CPPResolver/API/Class.hpp"
#include "IL2CPPResolver/API/Domain.hpp"
#include "IL2CPPResolver/API/String.hpp"
#include "IL2CPPResolver/API/Thread.hpp"
#include "IL2CPPResolver/Unity/API/Transform.hpp"
#include "module/ModuleRegistrar.hpp"
#include <cmath>

namespace zr {

SCPModule::SCPModule() : Module("SCPModule", ModuleWindowType::COMBAT) {}

SCPModule *SCPModule::getInstance() {
  static SCPModule instance;
  return &instance;
}

void SCPModule::PlayerClass_Update_Proxy(PlayerClass *this_ptr) {
  auto instance = getInstance();
  // 调用原始函数
  instance->playerClassUpdateHook.call(this_ptr);
  // 执行模块逻辑
  instance->onPlayerClassUpdate(this_ptr);
}

void SCPModule::PlayerClass_SendToServer_Proxy(
    PlayerClass *this_ptr, Unity::System_String *func,
    Unity::il2cppArray<Unity::il2cppObject *> *args) {
  getInstance()->onPlayerClassSendToServer(this_ptr, func, args);
}

void SCPModule::onPlayerClassUpdate(PlayerClass *this_ptr) {
  if (!autoAttackEnabled) {
    return;
  }

  auto localPlayer = LocalPlayer::getInstance();
  if (!localPlayer || !localPlayer->isInit()) {
    return;
  }

  auto playerClass = localPlayer->getPlayerClass();
  if (playerClass != this_ptr) {
    return;
  }

  // 帧间隔检查
  frameCounter++;
  if (frameCounter < attackInterval) {
    return;
  }
  frameCounter = 0;

  // 检查当前玩家的类是否具有ShootServer方法
  auto klass = ((Unity::il2cppObject *)this_ptr)->m_pClass;
  if (!IL2CPP::Class::Utils::GetMethodPointer(
          klass, "ShootServer",
          {"System.Int16", "UnityEngine.Vector3", "UnityEngine.Vector3"})) {
    return; // 如果没有攻击方法，则不执行后续逻辑
  }

  auto localTransform = localPlayer->GetTransform();
  if (!localTransform) {
    return;
  }
  Unity::Vector3 localPos = localTransform->GetPosition();
  std::string localTeamID = playerClass->getTeamID();

  auto players = Player::getAllPlayers();
  for (auto &player : players) {
    if (!player || player == localPlayer || !player->isInit() ||
        player->getHealth() <= 0) {
      continue;
    }

    auto targetPlayerClass = player->getPlayerClass();
    if (!targetPlayerClass) {
      continue;
    }

    std::string targetTeamID = targetPlayerClass->getTeamID();
    if (targetTeamID == localTeamID ||
        targetTeamID.find("Spectator") != std::string::npos) {
      continue;
    }

    auto targetTransform = player->GetTransform();
    if (!targetTransform) {
      continue;
    }

    Unity::Vector3 targetPos = targetTransform->GetPosition();
    float distance = Unity::Vector3::Distance(localPos, targetPos);

    if (distance <= attackRange) {
      // 构造参数
      static auto int16Class = IL2CPP::Class::Find("System.Int16");
      static auto vector3Class = IL2CPP::Class::Find("UnityEngine.Vector3");

      if (int16Class && vector3Class) {
        std::vector<Unity::il2cppObject *> args(3);
        int16_t targetID = player->getPlayerID();
        Unity::Vector3 pos = targetPos; // 也可以是localPos，根据需要调整
        Unity::Vector3 rot = {0, 0, 0};

        args[0] = Unity::il2cppObject::BoxObject(int16Class, &targetID);
        args[1] = Unity::il2cppObject::BoxObject(vector3Class, &pos);
        args[2] = Unity::il2cppObject::BoxObject(vector3Class, &rot);

        for (int i = 0; i < autoAttackCount; ++i) {
          this_ptr->sendToServer("ShootServer", args);
        }
      }
      // 找到一个目标攻击后就可以跳出循环，避免一次性攻击多个
      break;
    }
  }
}

void SCPModule::onPlayerClassSendToServer(
    PlayerClass *this_ptr, Unity::System_String *func,
    Unity::il2cppArray<Unity::il2cppObject *> *args) {
  if (superAttackEnabled && func && func->ToString() == "ShootServer") {
    auto klass = ((Unity::il2cppObject *)this_ptr)->m_pClass;
    if (IL2CPP::Class::Utils::GetMethodPointer(
            klass, "ShootServer",
            {"System.Int16", "UnityEngine.Vector3", "UnityEngine.Vector3"})) {
      for (int i = 0; i < superAttackCount - 1; ++i) {
        playerClassSendToServerHook.call(this_ptr, func, args);
      }
    }
  }
  // 调用原始函数
  playerClassSendToServerHook.call(this_ptr, func, args);
}

std::optional<std::string> SCPModule::enable() {
  auto thread = IL2CPP::Thread::Attach(IL2CPP::Domain::Get());

  void *updateAddr = IL2CPP::Class::Utils::GetMethodPointer(
      "Akequ.Base.PlayerClass", "Update", 0);
  if (!updateAddr) {
    IL2CPP::Thread::Detach(thread);
    return "Failed to find PlayerClass::Update";
  }
  auto updateHookResult =
      playerClassUpdateHook.create(updateAddr, PlayerClass_Update_Proxy);
  if (!updateHookResult) {
    IL2CPP::Thread::Detach(thread);
    return "Failed to hook PlayerClass::Update";
  }
  playerClassUpdateHook = std::move(updateHookResult.value());

  void *sendToServerAddr = IL2CPP::Class::Utils::GetMethodPointer(
      "Akequ.Base.PlayerClass", "SendToServer", 2);
  if (!sendToServerAddr) {
    IL2CPP::Thread::Detach(thread);
    return "Failed to find PlayerClass::SendToServer";
  }
  auto sendToServerHookResult = playerClassSendToServerHook.create(
      sendToServerAddr, PlayerClass_SendToServer_Proxy);
  if (!sendToServerHookResult) {
    IL2CPP::Thread::Detach(thread);
    return "Failed to hook PlayerClass::SendToServer";
  }
  playerClassSendToServerHook = std::move(sendToServerHookResult.value());

  IL2CPP::Thread::Detach(thread);
  return std::nullopt;
}

std::optional<std::string> SCPModule::disable() {
  playerClassUpdateHook.reset();
  playerClassSendToServerHook.reset();
  return std::nullopt;
}

std::optional<std::string> SCPModule::drawGUI() {
  ImGui::SeparatorText("Auto Attack");
  ImGui::Checkbox("Enable Auto Attack", &autoAttackEnabled);
  ImGui::SliderFloat("Attack Range (m)", &attackRange, 1.0f, 100.0f, "%.1f");
  ImGui::SliderInt("Attack Interval (frames)", &attackInterval, 1, 1000);
  ImGui::SliderInt("Auto Attack Count", &autoAttackCount, 1, 100);

  ImGui::SeparatorText("Super Attack");
  ImGui::Checkbox("Enable Super Attack", &superAttackEnabled);
  ImGui::SliderInt("Super Attack Count", &superAttackCount, 1, 100);

  return std::nullopt;
}

std::optional<std::string> SCPModule::toJson(nlohmann::json &json) const {
  json["autoAttackEnabled"] = autoAttackEnabled;
  json["attackRange"] = attackRange;
  json["attackInterval"] = attackInterval;
  json["autoAttackCount"] = autoAttackCount;
  json["superAttackEnabled"] = superAttackEnabled;
  json["superAttackCount"] = superAttackCount;
  return std::nullopt;
}

std::optional<std::string> SCPModule::fromJson(const nlohmann::json &json) {
  autoAttackEnabled = json.value("autoAttackEnabled", false);
  attackRange = json.value("attackRange", 10.0f);
  attackInterval = json.value("attackInterval", 10);
  autoAttackCount = json.value("autoAttackCount", 1);
  superAttackEnabled = json.value("superAttackEnabled", false);
  superAttackCount = json.value("superAttackCount", 1);
  return std::nullopt;
}

REGISTER_MODULE(SCPModule, SCPModule::getInstance());

} // namespace zr