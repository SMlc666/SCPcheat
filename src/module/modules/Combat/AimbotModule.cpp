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
  weaponShootHandle = weaponShootServerCallbacks.append(
      [this](Weapon *w) { this->onWeaponShoot(w); });
  renderHandle = zr::ImGuiCallback.append([this]() { this->renderFov(); });
  return std::nullopt;
}

std::optional<std::string> AimbotModule::disable() {
  getLogger().info("AimbotModule disabled.");
  weaponShootServerCallbacks.remove(weaponShootHandle);
  zr::ImGuiCallback.remove(renderHandle);
  return std::nullopt;
}

std::optional<std::string> AimbotModule::drawGUI() {
  ImGui::Checkbox("Enable Aimbot", &aimbotEnabled);
  ImGui::Separator();
  const char *items[] = {"Distance", "Health"};
  int currentItem = static_cast<int>(aimPriority);
  if (ImGui::Combo("Aim Priority", &currentItem, items, IM_ARRAYSIZE(items))) {
    aimPriority = static_cast<AimPriority>(currentItem);
  }
  ImGui::SliderFloat("Aim FOV", &aimFov, 1.0f, 1000.0f, "%.1f");
  ImGui::Separator();
  ImGui::Checkbox("Draw FOV", &drawFov);
  ImGui::ColorEdit4("FOV Color", &fovColor.x);
  return std::nullopt;
}

void AimbotModule::onWeaponShoot(Weapon *weapon) {
  if (!aimbotEnabled) {
    return;
  }

  auto thread = IL2CPP::Thread::Attach(IL2CPP::Domain::Get());

  auto localPlayer = LocalPlayer::getInstance();
  auto camera = Unity::Camera::GetMain();
  if (!localPlayer || !localPlayer->isInit() || !camera) {
    IL2CPP::Thread::Detach(thread);
    return;
  }

  auto players = Player::getAllPlayers();
  if (players.empty()) {
    IL2CPP::Thread::Detach(thread);
    return;
  }

  Player *bestTarget = nullptr;
  float minMetric = (std::numeric_limits<float>::max)();

  Unity::Vector3 localPlayerPos = localPlayer->GetTransform()->GetPosition();
  Unity::Vector2 screenCenter = {ImGui::GetIO().DisplaySize.x / 2.0f,
                                 ImGui::GetIO().DisplaySize.y / 2.0f};

  for (auto player : players) {
    if (!player || player == localPlayer || !player->isInit() ||
        player->getHealth() <= 0) {
      continue;
    }

    Unity::Vector3 playerPos = player->GetTransform()->GetPosition();
    Unity::Vector2 screenPos;

    if (camera->WorldToScreen(playerPos, screenPos)) {
      float dist_x = screenPos.x - screenCenter.x;
      float dist_y = screenPos.y - screenCenter.y;
      float screenDist = sqrtf(dist_x * dist_x + dist_y * dist_y);

      if (screenDist < aimFov) {
        float currentMetric = 0.0f;
        if (aimPriority == AimPriority::Distance) {
          Unity::Vector3 diff;
          diff.x = localPlayerPos.x - playerPos.x;
          diff.y = localPlayerPos.y - playerPos.y;
          diff.z = localPlayerPos.z - playerPos.z;
          currentMetric = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
        } else { // Health
          currentMetric = static_cast<float>(player->getHealth());
        }

        if (currentMetric < minMetric) {
          minMetric = currentMetric;
          bestTarget = player;
        }
      }
    }
  }

  if (bestTarget) {
    auto fpc = localPlayer->getFirstPersonController();
    if (fpc) {
      auto mouseLook = fpc->getMouseLook();
      if (mouseLook) {
        Unity::Vector3 targetPos = bestTarget->GetTransform()->GetPosition();
        targetPos.y += 0.8f; // Aim for the upper body/head area
        Unity::Vector3 cameraPos = camera->GetTransform()->GetPosition();

        Unity::Vector3 dir;
        dir.x = targetPos.x - cameraPos.x;
        dir.y = targetPos.y - cameraPos.y;
        dir.z = targetPos.z - cameraPos.z;

        float magnitude = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
        float yaw = atan2f(dir.x, dir.z) * (180.0f / M_PI);
        float pitch = -asinf(dir.y / magnitude) * (180.0f / M_PI);

        Unity::Quaternion targetRotation;
        targetRotation.Euler(pitch, yaw, 0);
        mouseLook->setCameraTargetRot(targetRotation);
      }
    }
  }

  IL2CPP::Thread::Detach(thread);
}

void AimbotModule::renderFov() {
  if (!aimbotEnabled || !drawFov) {
    return;
  }
  auto drawList = ImGui::GetBackgroundDrawList();
  auto &io = ImGui::GetIO();
  drawList->AddCircle(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f),
                      aimFov, ImGui::ColorConvertFloat4ToU32(fovColor), 64,
                      1.0f);
}

std::optional<std::string> AimbotModule::toJson(nlohmann::json &json) const {
  json["aimbotEnabled"] = aimbotEnabled;
  json["aimPriority"] = static_cast<int>(aimPriority);
  json["aimFov"] = aimFov;
  json["drawFov"] = drawFov;
  zr::toJson(json["fovColor"], fovColor);
  return std::nullopt;
}

std::optional<std::string> AimbotModule::fromJson(const nlohmann::json &json) {
  aimbotEnabled = json.value("aimbotEnabled", true);
  aimPriority = static_cast<AimPriority>(json.value("aimPriority", 0));
  aimFov = json.value("aimFov", 100.0f);
  drawFov = json.value("drawFov", true);
  zr::fromJson(
      json.value("fovColor", nlohmann::json::array({1.0f, 1.0f, 1.0f, 0.3f})),
      fovColor, ImVec4(1.0f, 1.0f, 1.0f, 0.3f));
  return std::nullopt;
}

REGISTER_MODULE(AimbotModule, AimbotModule::getInstance());

} // namespace zr