#pragma once

#include "IL2CPPResolver/Unity/API/Camera.hpp"
#include "draw/gui/gui.hpp"
#include "imgui.h"
#include "module/module.hpp"
#include "u3d/sdk/Actor/Player/LocalPlayer.hpp"
#include "u3d/sdk/Control/MouseLook.hpp"
#include "u3d/sdk/WeaponShoot.hpp"

namespace zr {

class AimbotModule : public Module {
public:
  enum class AimPriority { Distance, Health, DistanceToCrosshair };
  enum class AimMode { MouseLook, MouseMove };

private:
  AimMode aimMode = AimMode::MouseLook;
  int aimSmooth = 5;
  AimPriority aimPriority = AimPriority::Distance;
  float aimFov = 100.0f;
  bool drawFov = true;
  ImVec4 fovColor{1.0f, 1.0f, 1.0f, 0.3f};
  bool ignoreClassD = true;
  bool ignoreSCP999 = true;
  float maxDistance = 300.0f;
  bool raycast = true;
  
  WeaponDoShootCallbackList::Handle weaponShootHandle;
  eventpp::CallbackList<void()>::Handle renderHandle;

  void onWeaponShoot(Weapon *weapon);
  void renderFov();

public:
  AimbotModule();
  static AimbotModule *getInstance();

  std::optional<std::string> enable() override;
  std::optional<std::string> disable() override;
  std::optional<std::string> drawGUI() override;
  std::optional<std::string> toJson(nlohmann::json &json) const override;
  std::optional<std::string> fromJson(const nlohmann::json &json) override;
};

} // namespace zr