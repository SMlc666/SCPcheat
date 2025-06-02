#include "WeaponModule.hpp"
#include "IL2CPPResolver/API/Class.hpp"
#include "imgui.h"
#include "module/ModuleRegistrar.hpp"
#include "safetyhook/inline_hook.hpp"
#include <optional>

namespace zr {
safetyhook::InlineHook Weapon_UpdateHook;
WeaponModule *WeaponModule::getInstance() {
  static WeaponModule instance;
  return &instance;
}
void Weapon_UpdateProxy(IL2CPP::CClass *Weapon) {
  if (WeaponModule::getInstance()->isEnabled()) {
    Weapon->SetMemberValue("shootSpeed",
                           WeaponModule::getInstance()->shootSpeed);
    Weapon->SetMemberValue("recoil", WeaponModule::getInstance()->recoil);
    Weapon->SetMemberValue("spread", WeaponModule::getInstance()->spread);
    Weapon->SetMemberValue("size_comp", WeaponModule::getInstance()->size_comp);
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
    getLogger().error("WeaponUpdateAddr:{:p}", Weapon_UpdateAddr);
  }
  auto result = Weapon_UpdateHook.create(Weapon_UpdateAddr, Weapon_UpdateProxy);
  if (!result.has_value()) {
    getLogger().error("hook error");
    return std::nullopt;
  } else {
    Weapon_UpdateHook = std::move(result.value());
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
  // TODO: 绘制GUI界面
  return std::nullopt;
}
std::optional<std::string> WeaponModule::toJson(nlohmann::json &json) const {
  json["shootSpeed"] = shootSpeed;
  json["recoil"] = recoil;
  json["spread"] = spread;
  json["size_comp"] = size_comp;
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
  return std::nullopt;
}
REGISTER_MODULE(WeaponModule, WeaponModule::getInstance());
} // namespace zr