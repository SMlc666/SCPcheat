#include "WeaponModule.hpp"
#include "IL2CPPResolver/API/Class.hpp"
#include "IL2CPPResolver/API/String.hpp"
#include "IL2CPPResolver/Unity/API/Transform.hpp"
#include "IL2CPPResolver/Unity/Structures/System_String.hpp"
#include "IL2CPPResolver/Unity/Structures/Vector3.hpp"
#include "IL2CPPResolver/Unity/Structures/il2cpp.hpp"
#include "IL2CPPResolver/Unity/Structures/il2cppArray.hpp"
#include "imgui.h"
#include "module/ModuleRegistrar.hpp"
#include "safetyhook/inline_hook.hpp"
#include "u3d/sdk/Actor/Player/LocalPlayer.hpp"
#include "u3d/sdk/Base/Item/Item.hpp"
#include "u3d/sdk/Base/Item/Weapon/Weapon.hpp"
#include <cstdint>
#include <mutex>
#include <optional>

namespace zr {
safetyhook::InlineHook Weapon_UpdateHook;
safetyhook::InlineHook Weapon_SendToServerHook;

WeaponModule *WeaponModule::getInstance() {
  static WeaponModule instance;
  return &instance;
}
void Weapon_SendToServerProxy(Item *item, Unity::System_String *name,
                              Unity::il2cppArray<Unity::il2cppObject *> *args) {
  static std::once_flag flag;
  std::call_once(flag, [&]() {
    WeaponModule::getInstance()->getLogger().info(
        "Installing Weapon_SendToServer hook {}", name->ToString());
  });
  if (WeaponModule::getInstance()->isEnabled()) {
    if (name && name->ToString() == "ReloadServer") {
      if (args && args->m_uMaxLength > 0) {
        auto clazz = IL2CPP::Class::Find("System.Single");
        if (clazz) {
          float value = WeaponModule::getInstance()->reloadingTime;
          args->operator[](0) = Unity::il2cppObject::BoxObject(clazz, &value);
          WeaponModule::getInstance()->getLogger().info(
              "args type : {}.{}", args->operator[](0)->m_pClass->m_pNamespace,
              args->operator[](0)->m_pClass->m_pName);
        } else {
          WeaponModule::getInstance()->getLogger().error(
              "failed to find System.Single class");
        }
      }
    }
    if (name && name->ToString() == "ShootServer" &&
        WeaponModule::getInstance()->superBullet) {
      for (int i = 0; i < WeaponModule::getInstance()->bulletCount; i++) {
        Weapon_SendToServerHook.call(item, name, args);
      }
    }
  }

  return Weapon_SendToServerHook.call(item, name, args);
}

void Weapon_UpdateProxy(Weapon *Weapon) {
  LocalPlayer *localPlayer = LocalPlayer::getInstance();
  if (WeaponModule::getInstance()->isEnabled() &&
      Weapon->getPlayer() == localPlayer) {
    if (WeaponModule::getInstance()->changeShootSpeed) {
      Weapon->SetMemberValue("shootSpeed",
                             WeaponModule::getInstance()->shootSpeed);
    }
    Weapon->SetMemberValue("recoil", WeaponModule::getInstance()->recoil);
    Weapon->SetMemberValue("spread", WeaponModule::getInstance()->spread);
    Weapon->SetMemberValue("size_comp", WeaponModule::getInstance()->size_comp);
    Weapon->SetMemberValue("isReloading",
                           WeaponModule::getInstance()->isReloading);
    Weapon->SetMemberValue("anticheat", WeaponModule::getInstance()->anticheat);
    Weapon->SetMemberValue("friendlyFire",
                           WeaponModule::getInstance()->friendlyFire);
    if (WeaponModule::getInstance()->friendlyFire) {
      static auto clazz = IL2CPP::Class::Find("System.Boolean");
      std::vector<Unity::il2cppObject *> args(3);
      args[0] = IL2CPP::String::New("System.Boolean");
      args[1] = IL2CPP::String::New("friendlyFire");
      args[2] = Unity::il2cppObject::BoxObject(
          clazz, &WeaponModule::getInstance()->friendlyFire);
      Weapon->sendToServer("FieldSetter", args);
    }
    if (WeaponModule::getInstance()->infiniteAmmo) {
      static auto clazz = IL2CPP::Class::Find("System.UInt16");
      static auto clazz1 = IL2CPP::Class::Find("System.Single");
      if (clazz && clazz1) {
        auto args = std::vector<Unity::il2cppObject *>(2);
        uint16_t maxAmmo = 32767;
        uint16_t currentAmmo = 32766;
        if (auto obj1 = Unity::il2cppObject::BoxObject(clazz, &currentAmmo)) {
          args[0] = obj1;
        } else {
          WeaponModule::getInstance()->getLogger().error(
              "Failed to box System.UInt16 object");
        }

        if (auto obj2 = Unity::il2cppObject::BoxObject(clazz, &maxAmmo)) {
          args[1] = obj2;
        } else {
          WeaponModule::getInstance()->getLogger().error(
              "Failed to box System.UInt16 object");
        }
        Weapon->sendToServer("GetClip", args);
        auto args1 = std::vector<Unity::il2cppObject *>(1);
        float time = 0.0f;
        if (auto obj3 = Unity::il2cppObject::BoxObject(clazz1, &time)) {
          args1[0] = obj3;
        } else {
          WeaponModule::getInstance()->getLogger().error(
              "Failed to box System.Single object");
        }
        Weapon->sendToServer("ReloadServer", args1);
      } else {
        std::once_flag flag;
        std::call_once(flag, [&]() {
          WeaponModule::getInstance()->getLogger().error(
              "Failed to find System.UInt16 class or System.Single class");
        });
      }
    }
    if (WeaponModule::getInstance()->autoAttack &&
        !WeaponModule::getInstance()->spoofYourAttack) {
      static auto clazz = IL2CPP::Class::Find("System.Int16");
      static auto clazz1 = IL2CPP::Class::Find("UnityEngine.Vector3");
      if (clazz && clazz1) {
        auto players = Player::getAllPlayers();
        for (auto player : players) {
          auto args = std::vector<Unity::il2cppObject *>(2);
          int16_t playerID = player->getPlayerID();
          Unity::Vector3 direction = {0, 0, 0};
          args[0] = Unity::il2cppObject::BoxObject(clazz, &playerID);
          args[1] = Unity::il2cppObject::BoxObject(clazz1, &direction);
          for (int i = 0; i < WeaponModule::getInstance()->attackCount; i++) {
            Weapon->sendToServer("ShootServer", args);
          }
        }
      } else {
        std::once_flag flag;
        std::call_once(flag, [&]() {
          WeaponModule::getInstance()->getLogger().error(
              "Failed to find System.Int16 class or UnityEngine.Vector3 class");
        });
      }
    }
  }
  if (Weapon->getPlayer() && Weapon->getPlayer() != localPlayer &&
      WeaponModule::getInstance()->isEnabled() &&
      WeaponModule::getInstance()->spoofYourAttack) {
    static auto clazz = IL2CPP::Class::Find("System.Int16");
    static auto clazz1 = IL2CPP::Class::Find("UnityEngine.Vector3");
    if (clazz && clazz1) {
      auto players = Player::getAllPlayers();
      for (auto player : players) {
        auto args = std::vector<Unity::il2cppObject *>(2);
        int16_t playerID = player->getPlayerID();
        Unity::Vector3 direction = {0, 0, 0};
        args[0] = Unity::il2cppObject::BoxObject(clazz, &playerID);
        args[1] = Unity::il2cppObject::BoxObject(clazz1, &direction);
        for (int i = 0; i < WeaponModule::getInstance()->attackCount; i++) {
          Weapon->sendToServer("ShootServer", args);
        }
      }
    } else {
      std::once_flag flag;
      std::call_once(flag, [&]() {
        WeaponModule::getInstance()->getLogger().error(
            "Failed to find System.Int16 class or UnityEngine.Vector3 class");
      });
    }
  }
  Weapon_UpdateHook.call(Weapon);
}
std::optional<std::string> WeaponModule::load() {
  // TODO: 加载模块资源/初始化
  void *Weapon_UpdateAddr =
      IL2CPP::Class::Utils::GetMethodPointer("Akequ.Base.Weapon", "Update");
  if (!Weapon_UpdateAddr) {
    getLogger().error("cannot get WeaponUpdateAddr");
    return std::nullopt;
  } else {
    getLogger().info("WeaponUpdateAddr:{:p}", Weapon_UpdateAddr);
  }
  auto result = Weapon_UpdateHook.create(Weapon_UpdateAddr, Weapon_UpdateProxy);
  if (!result.has_value()) {
    getLogger().error("hook error");
    return std::nullopt;
  } else {
    Weapon_UpdateHook = std::move(result.value());
  }
  void *SendToServer_Addr = IL2CPP::Class::Utils::GetMethodPointer(
      "Akequ.Base.Item", "SendToServer", 2); // 2个参数
  if (!SendToServer_Addr) {
    getLogger().error("cannot get SendToServer_Addr");
    return "Failed to get SendToServer address";
  } else {
    getLogger().info("SendToServer_Addr:{:p}", SendToServer_Addr);
  }

  // 创建并安装Hook
  auto result1 = Weapon_SendToServerHook.create(SendToServer_Addr,
                                                Weapon_SendToServerProxy);
  if (!result1) {
    getLogger().error("Failed to create hook for SendToServer");
    return "Hook creation failed";
  } else {
    Weapon_SendToServerHook = std::move(result1.value());
  }
  return std::nullopt;
}

std::optional<std::string> WeaponModule::enable() {
  // TODO: 启用模块功能
  return std::nullopt;
}

std::optional<std::string> WeaponModule::disable() {
  // TODO: 禁用模块功能
  return std::nullopt;
}

std::optional<std::string> WeaponModule::drawGUI() {
  ImGui::Checkbox("changeShootSpeed", &changeShootSpeed);
  ImGui::DragFloat("shootSpeed", &shootSpeed, 0.1, 0.0001, 2);
  ImGui::DragFloat("recoil", &recoil, 0.01, 0.01, 5);
  ImGui::DragFloat("spread", &spread, 0.01, 0.01, 5);
  ImGui::DragFloat("size_comp", &size_comp, 0.1, 0.01, 10);
  ImGui::DragFloat("reloadingTime", &reloadingTime, 0.01, 0.01, 10);
  ImGui::Checkbox("isReloading", &isReloading);
  ImGui::Checkbox("anticheat", &anticheat);
  ImGui::Checkbox("friendlyFire", &friendlyFire);
  ImGui::Checkbox("infiniteAmmo", &infiniteAmmo);
  ImGui::Checkbox("autoAttack", &autoAttack);
  ImGui::SliderInt("attackCount", &attackCount, 1, 300);
  ImGui::Checkbox("spoofYourAttack", &spoofYourAttack);
  ImGui::Checkbox("superBullet", &superBullet);
  ImGui::SliderInt("bulletCount", &bulletCount, 1, 100);
  return std::nullopt;
}
std::optional<std::string> WeaponModule::toJson(nlohmann::json &json) const {
  json["changeShootSpeed"] = changeShootSpeed;
  json["shootSpeed"] = shootSpeed;
  json["recoil"] = recoil;
  json["spread"] = spread;
  json["size_comp"] = size_comp;
  json["isReloading"] = isReloading;
  json["anticheat"] = anticheat;
  json["friendlyFire"] = friendlyFire;
  json["reloadingTime"] = reloadingTime;
  json["infiniteAmmo"] = infiniteAmmo;
  json["autoAttack"] = autoAttack;
  json["attackCount"] = attackCount;
  json["spoofYourAttack"] = spoofYourAttack;
  json["superBullet"] = superBullet;
  json["bulletCount"] = bulletCount;
  return std::nullopt;
}
std::optional<std::string> WeaponModule::fromJson(const nlohmann::json &json) {
  if (json.contains("changeShootSpeed") &&
      json["changeShootSpeed"].is_boolean()) {
    changeShootSpeed = json["changeShootSpeed"].get<bool>();
  } else {
    return "Missing 'changeShootSpeed' in JSON";
  }
  if (json.contains("shootSpeed") && json["shootSpeed"].is_number()) {
    shootSpeed = json["shootSpeed"].get<float>();
  } else {
    return "Missing 'shootSpeed' in JSON";
  }
  if (json.contains("recoil") && json["recoil"].is_number()) {
    recoil = json["recoil"].get<float>();
  } else {
    return "Missing 'recoil' in JSON";
  }
  if (json.contains("spread") && json["spread"].is_number()) {
    spread = json["spread"].get<float>();
  } else {
    return "Missing 'spread' in JSON";
  }
  if (json.contains("size_comp") && json["size_comp"].is_number()) {
    size_comp = json["size_comp"].get<float>();
  } else {
    return "Missing 'size_comp' in JSON";
  }
  if (json.contains("isReloading") && json["isReloading"].is_boolean()) {
    isReloading = json["isReloading"].get<bool>();
  } else {
    return "Missing 'isReloading' in JSON";
  }
  if (json.contains("anticheat") && json["anticheat"].is_boolean()) {
    anticheat = json["anticheat"].get<bool>();
  } else {
    return "Missing 'anticheat' in JSON";
  }
  if (json.contains("friendlyFire") && json["friendlyFire"].is_boolean()) {
    friendlyFire = json["friendlyFire"].get<bool>();
  } else {
    return "Missing 'friendlyFire' in JSON";
  }
  if (json.contains("reloadingTime") && json["reloadingTime"].is_number()) {
    reloadingTime = json["reloadingTime"].get<float>();
  } else {
    return "Missing 'reloadingTime' in JSON";
  }
  if (json.contains("infiniteAmmo") && json["infiniteAmmo"].is_boolean()) {
    infiniteAmmo = json["infiniteAmmo"].get<bool>();
  } else {
    return "Missing 'infiniteAmmo' in JSON";
  }
  if (json.contains("autoAttack") && json["autoAttack"].is_boolean()) {
    autoAttack = json["autoAttack"].get<bool>();
  } else {
    return "Missing 'autoAttack' in JSON";
  }
  if (json.contains("attackCount") && json["attackCount"].is_number()) {
    attackCount = json["attackCount"].get<int>();
  } else {
    return "Missing 'attackCount' in JSON";
  }
  if (json.contains("spoofYourAttack") &&
      json["spoofYourAttack"].is_boolean()) {
    spoofYourAttack = json["spoofYourAttack"].get<bool>();
  } else {
    return "Missing'spoofYourAttack' in JSON";
  }
  if (json.contains("superBullet") && json["superBullet"].is_boolean()) {
    superBullet = json["superBullet"].get<bool>();
  } else {
    return "Missing'superBullet' in JSON";
  }
  if (json.contains("bulletCount") && json["bulletCount"].is_number()) {
    bulletCount = json["bulletCount"].get<int>();
  } else {
    return "Missing 'bulletCount' in JSON";
  }
  return std::nullopt;
}
REGISTER_MODULE(WeaponModule, WeaponModule::getInstance());
} // namespace zr