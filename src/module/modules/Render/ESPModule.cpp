#include "ESPModule.hpp"
#include "IL2CPPResolver/API/Domain.hpp"
#include "IL2CPPResolver/API/Thread.hpp"
#include "IL2CPPResolver/Unity/API/Camera.hpp"
#include "IL2CPPResolver/Unity/API/Transform.hpp"
#include "imgui.h"
#include "imgui_internal.h" // For ImGui::GetCurrentWindow() if needed, or other internal bits
#include "log/log.hpp" // For logging
#include "module/ModuleRegistrar.hpp"
#include "u3d/sdk/Actor/Player/LocalPlayer.hpp"
#include "u3d/sdk/Actor/Player/Player.hpp"
#include <algorithm> // For std::min/max
#include <cmath>
#include <fmt/format.h> // For string formatting if needed
#include <gsl/gsl>

// Helper for color serialization (optional, can do inline)
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

ESPModule::ESPModule() : Module("ESPModule", ModuleWindowType::RENDER) {
  getLogger().info("ESPModule initialized with default settings, including new "
                   "offset/sizing parameters.");
}

ESPModule *ESPModule::getInstance() {
  static ESPModule *instance = new ESPModule();
  return instance;
}

std::optional<std::string> ESPModule::load() {
  getLogger().info("ESPModule::load called.");
  return std::nullopt;
}

std::optional<std::string> ESPModule::enable() {

  espRenderCallbackHandle =
      zr::ImGuiCallback.append([this]() { this->renderESP(); });
  getLogger().info("ESPModule enabled and render callback registered.");
  // Module::enableModule(); // Call base class method to set misEnabled = true
  return Module::enableModule(); // This will set misEnabled and call our
                                 // enable() if not already called. Actually,
                                 // Module::enableModule() calls the virtual
                                 // enable(), so we just need to set misEnabled.
                                 // The base Module::enableModule() handles the
                                 // misEnabled flag.
}

std::optional<std::string> ESPModule::disable() {

  zr::ImGuiCallback.remove(espRenderCallbackHandle);
  espRenderCallbackHandle = {}; // Reset handle
  getLogger().info("ESPModule render callback unregistered.");

  // Module::disableModule(); // Call base class method to set misEnabled =
  // false
  return Module::disableModule();
}

std::optional<std::string> ESPModule::drawGUI() {
  ImGui::Checkbox("Show Player Names", &showPlayerNames);
  ImGui::Checkbox("Show Player Boxes", &showPlayerBoxes);
  ImGui::Checkbox("Show Player Health", &showPlayerHealth);
  ImGui::Checkbox("Show Health Text", &showPlayerHealthText);
  ImGui::Checkbox("Show Player Distance", &showPlayerDistance);
  ImGui::Checkbox("Show Player ClassName", &showPlayerClassName);
  ImGui::SliderFloat("Max ESP Distance", &maxESPDistance, 0.0f, 10000.0f,
                     "%.1f m");
  ImGui::SeparatorText("Colors");
  ImGui::ColorEdit4("Box Color", &enemyBoxColor.x);
  ImGui::ColorEdit4("Name Color", &enemyNameColor.x);
  ImGui::ColorEdit4("ClassName Color", &enemyClassNameColor.x);
  ImGui::ColorEdit4("Distance Color", &enemyDistanceColor.x);
  ImGui::ColorEdit4("Health Bar FG", &healthBarFgColor.x);
  ImGui::ColorEdit4("Health Bar BG", &healthBarBgColor.x);
  ImGui::ColorEdit4("Health Text Color", &healthTextColor.x);

  return std::nullopt;
}

std::optional<std::string> ESPModule::toJson(nlohmann::json &json) const {
  json["showPlayerNames"] = showPlayerNames;
  json["showPlayerBoxes"] = showPlayerBoxes;
  json["showPlayerHealth"] = showPlayerHealth;
  json["showPlayerDistance"] = showPlayerDistance;
  json["showPlayerClassName"] = showPlayerClassName;
  json["showPlayerHealthText"] = showPlayerHealthText;
  json["maxESPDistance"] = maxESPDistance;
  zr::toJson(json["enemyBoxColor"], enemyBoxColor);
  zr::toJson(json["enemyNameColor"], enemyNameColor);
  zr::toJson(json["enemyClassNameColor"], enemyClassNameColor);
  zr::toJson(json["enemyDistanceColor"], enemyDistanceColor);
  zr::toJson(json["healthBarFgColor"], healthBarFgColor);
  zr::toJson(json["healthBarBgColor"], healthBarBgColor);
  zr::toJson(json["healthTextColor"], healthTextColor);
  return std::nullopt;
}

std::optional<std::string> ESPModule::fromJson(const nlohmann::json &json) {
  showPlayerNames = json.value("showPlayerNames", true);
  showPlayerBoxes = json.value("showPlayerBoxes", true);
  showPlayerHealth = json.value("showPlayerHealth", true);
  showPlayerDistance = json.value("showPlayerDistance", true);
  showPlayerClassName = json.value("showPlayerClassName", true);
  showPlayerHealthText = json.value("showPlayerHealthText", true);
  maxESPDistance = json.value("maxESPDistance", 200.0f);
  zr::fromJson(json.value("enemyBoxColor",
                          nlohmann::json::array({1.0f, 0.0f, 0.0f, 1.0f})),
               enemyBoxColor, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
  zr::fromJson(json.value("enemyNameColor",
                          nlohmann::json::array({1.0f, 1.0f, 1.0f, 1.0f})),
               enemyNameColor, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
  zr::fromJson(json.value("enemyClassNameColor",
                          nlohmann::json::array({1.0f, 1.0f, 1.0f, 1.0f})),
               enemyClassNameColor, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
  zr::fromJson(json.value("enemyDistanceColor",
                          nlohmann::json::array({1.0f, 1.0f, 1.0f, 1.0f})),
               enemyDistanceColor, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
  zr::fromJson(json.value("healthBarFgColor",
                          nlohmann::json::array({0.0f, 1.0f, 0.0f, 1.0f})),
               healthBarFgColor, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
  zr::fromJson(json.value("healthBarBgColor",
                          nlohmann::json::array({0.5f, 0.5f, 0.5f, 0.7f})),
               healthBarBgColor, ImVec4(0.5f, 0.5f, 0.5f, 0.7f));
  zr::fromJson(json.value("healthTextColor",
                          nlohmann::json::array({1.0f, 1.0f, 1.0f, 1.0f})),
               healthTextColor, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
  LogInstance::getMainLogger().info(
      "ESPModule configuration loaded from JSON.");
  return std::nullopt;
}

void ESPModule::renderESP() {
  auto thread = IL2CPP::Thread::Attach(IL2CPP::Domain::Get());

  // Get the main camera
  auto camera = Unity::Camera::GetMain();
  if (!camera) {
    IL2CPP::Thread::Detach(thread);
    return;
  }

  // Get the local player
  auto localPlayer = zr::LocalPlayer::getInstance();
  if (!localPlayer || !localPlayer->isInit()) {
    IL2CPP::Thread::Detach(thread);
    return;
  }

  auto localtransform = localPlayer->GetTransform();
  // Get all players
  auto players = zr::Player::getAllPlayers();
  if (players.empty()) {
    IL2CPP::Thread::Detach(thread);
    return;
  }

  auto drawList = ImGui::GetBackgroundDrawList();
  Unity::Vector3 localPlayerPos = localtransform->GetPosition();

  for (auto player : players) {
    if (!player || player == localPlayer || !player->isInit()) {
      continue;
    }

    auto playerTransform = player->GetTransform();
    if (!playerTransform) {
      continue;
    }

    Unity::Vector3 playerPos = playerTransform->GetPosition();
    Unity::Vector3 diff;
    diff.x = localPlayerPos.x - playerPos.x;
    diff.y = localPlayerPos.y - playerPos.y;
    diff.z = localPlayerPos.z - playerPos.z;
    // Length() returns the squared length, so we need to take the square root.
    float distance = sqrtf(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);

    if (distance > maxESPDistance) {
      continue;
    }

    // Define player bounds (feet and head)
    Unity::Vector3 feetPos = playerPos;
    Unity::Vector3 headPos = feetPos;
    headPos.y += 1.8f; // Approximate height

    Unity::Vector2 screenFeetPos, screenHeadPos;

    // World to screen conversion
    if (camera->WorldToScreen(feetPos, screenFeetPos) &&
        camera->WorldToScreen(headPos, screenHeadPos)) {

      float height = std::abs(screenHeadPos.y - screenFeetPos.y);
      float width = height / 2.0f;

      ImVec2 topLeft(screenHeadPos.x - width / 2, screenHeadPos.y);
      ImVec2 bottomRight(screenHeadPos.x + width / 2, screenFeetPos.y);

      // Ensure top-left is actually top-left
      if (topLeft.y > bottomRight.y) {
        std::swap(topLeft.y, bottomRight.y);
      }

      // --- Draw Box ---
      if (showPlayerBoxes) {
        drawList->AddRect(topLeft, bottomRight,
                          ImGui::ColorConvertFloat4ToU32(enemyBoxColor));
      }

      // --- Draw Health Bar ---
      if (showPlayerHealth) {
        uint32_t health = player->getHealth();
        uint32_t maxHealth = player->getMaxHealth();
        if (maxHealth == 0)
          maxHealth = 100; // Avoid division by zero
        float healthPercent = static_cast<float>(health) / maxHealth;

        float barHeight = height;
        float barWidth = 3.0f;
        ImVec2 barTop(topLeft.x - barWidth - 2, topLeft.y);
        ImVec2 barBottom(topLeft.x - 2, bottomRight.y);

        // Background
        drawList->AddRectFilled(
            barTop, barBottom,
            ImGui::ColorConvertFloat4ToU32(healthBarBgColor));

        // Foreground
        float healthBarHeight = barHeight * healthPercent;
        ImVec2 healthBarTop(barTop.x, barBottom.y - healthBarHeight);
        drawList->AddRectFilled(
            healthBarTop, barBottom,
            ImGui::ColorConvertFloat4ToU32(healthBarFgColor));

        // Health text
        if (showPlayerHealthText) {
          std::string hpText = fmt::format("{} / {}", health, maxHealth);
          ImVec2 hpSize = ImGui::CalcTextSize(hpText.c_str());
          ImVec2 hpPos(barBottom.x + 4.0f,
                       healthBarTop.y + (barHeight - hpSize.y) * 0.5f);
          drawList->AddText(hpPos,
                            ImGui::ColorConvertFloat4ToU32(healthTextColor),
                            hpText.c_str());
        }
      }

      // --- Draw Player Name ---
      if (showPlayerNames) {
        std::string name = player->getAccountName();
        ImVec2 textSize = ImGui::CalcTextSize(name.c_str());
        ImVec2 textPos(topLeft.x + (width - textSize.x) / 2,
                       topLeft.y - textSize.y - 2);
        drawList->AddText(textPos,
                          ImGui::ColorConvertFloat4ToU32(enemyNameColor),
                          name.c_str());
      }

      // Calculate vertical offset for text below the box
      float yOffset = bottomRight.y + 2;

      // --- Draw Class Name ---
      if (showPlayerClassName) {
        std::string className = player->getClassName();
        ImVec2 textSize = ImGui::CalcTextSize(className.c_str());
        ImVec2 textPos(topLeft.x + (width - textSize.x) / 2, yOffset);
        drawList->AddText(textPos,
                          ImGui::ColorConvertFloat4ToU32(enemyClassNameColor),
                          className.c_str());
        yOffset += textSize.y; // Move offset down for the next text
      }

      // --- Draw Distance ---
      if (showPlayerDistance) {
        std::string distStr = fmt::format("{:.1f}m", distance);
        ImVec2 textSize = ImGui::CalcTextSize(distStr.c_str());
        ImVec2 textPos(topLeft.x + (width - textSize.x) / 2, yOffset);
        drawList->AddText(textPos,
                          ImGui::ColorConvertFloat4ToU32(enemyDistanceColor),
                          distStr.c_str());
      }
    }
  }
  IL2CPP::Thread::Detach(thread);
}

REGISTER_MODULE(ESPModule, ESPModule::getInstance());

} // namespace zr