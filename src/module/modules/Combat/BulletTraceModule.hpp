#pragma once

#include "IL2CPPResolver/Unity/Structures/Quaternion.hpp"
#include "draw/draw.hpp"
#include "draw/gui/window/windows/ModuleWindowType.hpp"
#include "imgui.h"
#include "module/module.hpp"
#include "safetyhook.hpp"
#include "u3d/sdk/Actor/Player/LocalPlayer.hpp"
#include "u3d/sdk/Control/MouseLook.hpp"
#include "u3d/sdk/WeaponShoot.hpp"

namespace zr {

class BulletTraceModule : public Module {
public:
  enum class AimPriority { Distance, Health, DistanceToCrosshair };

private:
  // Config
  AimPriority aimPriority = AimPriority::Distance;
  float aimFov = 100.0f;
  bool ignoreClassD = true;
  float maxDistance = 300.0f;
  bool drawFov = true;
  ImVec4 fovColor{1.0f, 1.0f, 1.0f, 0.3f};

  // Hook
  static safetyhook::InlineHook UpdateHook;
  static void Weapon_Update_Hooked(Weapon *m_pWeapon);
  void renderFov();
  eventpp::CallbackList<void()>::Handle renderHandle;

public:
  BulletTraceModule();
  ~BulletTraceModule();
  static BulletTraceModule *getInstance();

  std::optional<std::string> load() override;
  std::optional<std::string> enable() override;
  std::optional<std::string> disable() override;
  std::optional<std::string> drawGUI() override;
  std::optional<std::string> toJson(nlohmann::json &json) const override;
  std::optional<std::string> fromJson(const nlohmann::json &json) override;
};

} // namespace zr