#include "AimbotModule.hpp"
#include "IL2CPPResolver/API/Domain.hpp"
#include "IL2CPPResolver/API/Thread.hpp"
#include "IL2CPPResolver/Unity/API/Transform.hpp"
#include "draw/draw.hpp"
#include "log/log.hpp"
#include "module/ModuleRegistrar.hpp"
#include "u3d/sdk/Actor/Player/Player.hpp"
#include "u3d/sdk/Control/FirstPersonController.hpp"

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

#include <limits>

// Helper for color serialization

namespace {
void MouseMove(float tarx, float tary, float X, float Y, int smooth) {
  float ScreenCenterX = (X / 2);
  float ScreenCenterY = (Y / 2);
  float TargetX = 0;
  float TargetY = 0;

  if (tarx != 0) {
    if (tarx > ScreenCenterX) {
      TargetX = -(ScreenCenterX - tarx);
      TargetX /= smooth;
      if (TargetX + ScreenCenterX > ScreenCenterX * 2)
        TargetX = 0;
    }

    if (tarx < ScreenCenterX) {
      TargetX = tarx - ScreenCenterX;
      TargetX /= smooth;
      if (TargetX + ScreenCenterX < 0)
        TargetX = 0;
    }
  }

  if (tary != 0) {
    if (tary > ScreenCenterY) {
      TargetY = -(ScreenCenterY - tary);
      TargetY /= smooth;
      if (TargetY + ScreenCenterY > ScreenCenterY * 2)
        TargetY = 0;
    }

    if (tary < ScreenCenterY) {
      TargetY = tary - ScreenCenterY;
      TargetY /= smooth;
      if (TargetY + ScreenCenterY < 0)
        TargetY = 0;
    }
  }
  mouse_event(MOUSEEVENTF_MOVE, static_cast<DWORD>(TargetX),
              static_cast<DWORD>(TargetY), NULL, NULL);
}

} // namespace
namespace zr {
static void toJson(nlohmann::json &j, const ImVec4 &color) {
  j = {color.x, color.y, color.z, color.w};
}

static void fromJson(const nlohmann::json &j, ImVec4 &color,
                     const ImVec4 &default_color) {
  if (j.is_array() && j.size() == 4) {
    color.x = j[0].get<float>();
    color.y = j[1].get<float>();
    color.z = j[2].get<float>();
    color.w = j[3].get<float>();
  } else {
    color = default_color;
  }
}
} // namespace zr

namespace zr {

AimbotModule::AimbotModule()
    : Module("AimbotModule", ModuleWindowType::COMBAT) {}

AimbotModule *AimbotModule::getInstance() {
  static AimbotModule instance;
  return &instance;
}

std::optional<std::string> AimbotModule::enable() {
  getLogger().info("AimbotModule enabled.");
  weaponShootHandle = weaponDoShootCallbacks.append(
      [this](Weapon *w) { this->onWeaponShoot(w); });
  renderHandle = zr::ImGuiCallback.append([this]() { this->renderFov(); });
  return std::nullopt;
}

std::optional<std::string> AimbotModule::disable() {
  getLogger().info("AimbotModule disabled.");
  weaponDoShootCallbacks.remove(weaponShootHandle);
  zr::ImGuiCallback.remove(renderHandle);
  return std::nullopt;
}

std::optional<std::string> AimbotModule::drawGUI() {
  ImGui::Separator();
  const char *items[] = {"Distance", "Health"};
  int currentItem = static_cast<int>(aimPriority);
  if (ImGui::Combo("Aim Priority", &currentItem, items, IM_ARRAYSIZE(items))) {
    aimPriority = static_cast<AimPriority>(currentItem);
  }

  const char *aimModeItems[] = {"MouseLook", "MouseMove"};
  int currentAimMode = static_cast<int>(aimMode);
  if (ImGui::Combo("Aim Mode", &currentAimMode, aimModeItems,
                   IM_ARRAYSIZE(aimModeItems))) {
    aimMode = static_cast<AimMode>(currentAimMode);
  }

  if (aimMode == AimMode::MouseMove) {
    ImGui::SliderInt("Aim Smooth", &aimSmooth, 1, 20);
  }

  ImGui::SliderFloat("Aim FOV", &aimFov, 1.0f, 1000.0f, "%.1f");
  ImGui::Separator();
  ImGui::Checkbox("Draw FOV", &drawFov);
  ImGui::ColorEdit4("FOV Color", &fovColor.x);
  return std::nullopt;
}

void AimbotModule::onWeaponShoot(Weapon *weapon) {
  auto localPlayer = LocalPlayer::getInstance();
  if (!localPlayer) {
    return;
  }
  auto localtransform = localPlayer->GetTransform();
  if (!localtransform) {
    return;
  }
  auto mainCamera = Unity::Camera::GetMain();
  if (!mainCamera) {
    return;
  }
  auto localclass = localPlayer->getPlayerClass();
  if (!localclass) {
    return;
  }
  std::string localteam = localclass->getTeamID();
  if (localteam.empty()) {
    return;
  }
  auto players = Player::getAllPlayers();
  Player *bestTarget = nullptr;
  float closestValue = std::numeric_limits<float>::max();

  auto &io = ImGui::GetIO();
  Unity::Vector2 screenCenter = {io.DisplaySize.x * 0.5f,
                                 io.DisplaySize.y * 0.5f};
  for (auto player : players) {
    if (!player || player == localPlayer || player->getHealth() <= 0 ||
        !player->isInit()) {
      continue;
    }

    auto playerTransform = player->GetTransform();
    if (!playerTransform) {
      continue;
    }
    auto playerclass = player->getPlayerClass();
    if (!playerclass) {
      continue;
    }
    std::string playeam = playerclass->getTeamID();
    if (playeam.empty()) {
      continue;
    }
    if (playeam == localteam) {
      continue;
    }
    if (playeam.find("Spectator") != std::string::npos) {
      continue;
    }
    Unity::Vector3 playerPosition = playerTransform->GetPosition();
    Unity::Vector2 screenPosition;

    if (mainCamera->WorldToScreen(playerPosition, screenPosition)) {
      float distanceToCenter =
          Unity::Vector2::Distance(screenPosition, screenCenter);

      if (distanceToCenter > aimFov) {
        continue;
      }

      if (aimPriority == AimPriority::Distance) {
        float distance = Unity::Vector3::Distance(
            localPlayer->GetTransform()->GetPosition(), playerPosition);
        if (distance < closestValue) {
          closestValue = distance;
          bestTarget = player;
        }
      } else if (aimPriority == AimPriority::Health) {
        if (player->getHealth() < closestValue) {
          closestValue = player->getHealth();
          bestTarget = player;
        }
      }
    }
  }

  if (bestTarget) {
    auto targetTransform = bestTarget->GetTransform();
    if (aimMode == AimMode::MouseLook) {
      Unity::Vector3 direction =
          targetTransform->GetPosition() - localtransform->GetPosition();
      Unity::Quaternion rotation = Unity::Quaternion::LookRotation(direction);

      auto fpc = localPlayer->getFirstPersonController();
      if (fpc) {
        auto mouseLook = fpc->getMouseLook();
        if (mouseLook) {
          mouseLook->setCharacterTargetRot(rotation);
          mouseLook->setCameraTargetRot(rotation);
        }
      }
    } else if (aimMode == AimMode::MouseMove) {
      Unity::Vector2 targetScreenPos;
      if (mainCamera->WorldToScreen(targetTransform->GetPosition(),
                                    targetScreenPos)) {
        MouseMove(targetScreenPos.x, targetScreenPos.y, io.DisplaySize.x,
                  io.DisplaySize.y, aimSmooth);
      }
    }
  }
}

void AimbotModule::renderFov() {
  if (!drawFov) {
    return;
  }
  auto drawList = ImGui::GetBackgroundDrawList();
  auto &io = ImGui::GetIO();
  drawList->AddCircle(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f),
                      aimFov, ImGui::ColorConvertFloat4ToU32(fovColor), 64,
                      1.0f);
}

std::optional<std::string> AimbotModule::toJson(nlohmann::json &json) const {
  json["aimPriority"] = static_cast<int>(aimPriority);
  json["aimMode"] = static_cast<int>(aimMode);
  json["aimSmooth"] = aimSmooth;
  json["aimFov"] = aimFov;
  json["drawFov"] = drawFov;
  zr::toJson(json["fovColor"], fovColor);
  return std::nullopt;
}

std::optional<std::string> AimbotModule::fromJson(const nlohmann::json &json) {
  aimPriority = static_cast<AimPriority>(json.value("aimPriority", 0));
  aimMode = static_cast<AimMode>(json.value("aimMode", 0));
  aimSmooth = json.value("aimSmooth", 5);
  aimFov = json.value("aimFov", 100.0f);
  drawFov = json.value("drawFov", true);
  zr::fromJson(
      json.value("fovColor", nlohmann::json::array({1.0f, 1.0f, 1.0f, 0.3f})),
      fovColor, ImVec4(1.0f, 1.0f, 1.0f, 0.3f));
  return std::nullopt;
}

REGISTER_MODULE(AimbotModule, AimbotModule::getInstance());

} // namespace zr