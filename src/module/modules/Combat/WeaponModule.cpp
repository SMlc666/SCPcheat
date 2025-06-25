#include "WeaponModule.hpp"
#include "IL2CPPResolver/API/Class.hpp"
#include "IL2CPPResolver/Unity/Structures/System_String.hpp"
#include "IL2CPPResolver/Unity/Structures/il2cpp.hpp"
#include "IL2CPPResolver/Unity/Structures/il2cppArray.hpp"
#include "imgui.h"
#include "module/ModuleRegistrar.hpp"
#include "safetyhook/inline_hook.hpp"
#include "u3d/sdk/Actor/Player/LocalPlayer.hpp"
#include "u3d/sdk/Base/Item/Item.hpp"
#include "u3d/sdk/Base/Item/Weapon/Weapon.hpp"
#include <cstdint>
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
  if (WeaponModule::getInstance()->isEnabled()) {
    if (name && name->ToString() == "ReloadServer") {
      if (args && args->m_uMaxLength > 0) {
        auto clazz = IL2CPP::Class::Find("System.Single");
        if (clazz) {
          float value = WeaponModule::getInstance()->reloadingTime;
          args->operator[](0) = Unity::il2cppObject::BoxObject(clazz, &value);
        } else {
          WeaponModule::getInstance()->getLogger().error(
              "failed to find System.Single class");
        }
      }
    }
  }

  return Weapon_SendToServerHook.call(item, name, args);
}

void Weapon_UpdateProxy(Weapon *Weapon) {
  LocalPlayer *localPlayer = LocalPlayer::getInstance();
  if (WeaponModule::getInstance()->isEnabled() &&
      Weapon->getPlayer() == localPlayer) {

    Weapon->SetMemberValue("shootSpeed",
                           WeaponModule::getInstance()->shootSpeed);
    Weapon->SetMemberValue("recoil", WeaponModule::getInstance()->recoil);
    Weapon->SetMemberValue("spread", WeaponModule::getInstance()->spread);
    Weapon->SetMemberValue("size_comp", WeaponModule::getInstance()->size_comp);
    if (WeaponModule::getInstance()->auto_reload) {
      Weapon->CallMethod<void>("ReloadServer", 0.0f);
    }
    Weapon->SetMemberValue("canShoot", WeaponModule::getInstance()->canShoot);
    Weapon->SetMemberValue("isReloading",
                           WeaponModule::getInstance()->isReloading);
    Weapon->SetMemberValue("anticheat", WeaponModule::getInstance()->anticheat);
    Weapon->SetMemberValue("friendlyFire",
                           WeaponModule::getInstance()->friendlyFire);
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
  if (!result) {
    getLogger().error("Failed to create hook for SendToServer");
    return "Hook creation failed";
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
  ImGui::DragFloat("shootSpeed", &shootSpeed, 0.1, 0.0001, 2);
  ImGui::DragFloat("recoil", &recoil, 0.01, 0.01, 5);
  ImGui::DragFloat("spread", &spread, 0.01, 0.01, 5);
  ImGui::DragFloat("size_comp", &size_comp, 0.1, 0.01, 10);
  ImGui::DragFloat("reloadingTime", &reloadingTime, 0.01, 0.01, 10);
  ImGui::Checkbox("auto_reload", &auto_reload);
  ImGui::Checkbox("canShoot", &canShoot);
  ImGui::Checkbox("isReloading", &isReloading);
  ImGui::Checkbox("anticheat", &anticheat);
  ImGui::Checkbox("friendlyFire", &friendlyFire);
  return std::nullopt;
}
std::optional<std::string> WeaponModule::toJson(nlohmann::json &json) const {
  json["shootSpeed"] = shootSpeed;
  json["recoil"] = recoil;
  json["spread"] = spread;
  json["size_comp"] = size_comp;
  json["auto_reload"] = auto_reload;
  json["canShoot"] = canShoot;
  json["isReloading"] = isReloading;
  json["anticheat"] = anticheat;
  json["friendlyFire"] = friendlyFire;
  json["reloadingTime"] = reloadingTime;
  return std::nullopt;
}
std::optional<std::string> WeaponModule::fromJson(const nlohmann::json &json) {
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
  if (json.contains("auto_reload") && json["auto_reload"].is_boolean()) {
    auto_reload = json["auto_reload"].get<bool>();
  } else {
    return "Missing 'auto_reload' in JSON";
  }
  if (json.contains("canShoot") && json["canShoot"].is_boolean()) {
    canShoot = json["canShoot"].get<bool>();
  } else {
    return "Missing 'canShoot' in JSON";
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

  return std::nullopt;
}
REGISTER_MODULE(WeaponModule, WeaponModule::getInstance());
} // namespace zr