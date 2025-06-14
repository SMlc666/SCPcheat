#pragma once

#include "IL2CPPResolver/Unity/API/Camera.hpp"
#include "IL2CPPResolver/Unity/Structures/Engine.hpp" // For Unity::Vector3
#include "draw/gui/gui.hpp"
#include "draw/gui/window/windows/ModuleWindowType.hpp"
#include "imgui.h"
#include "module/module.hpp"
#include "nlohmann/json.hpp"
#include "u3d/sdk/Actor/Player/LocalPlayer.hpp"
#include "u3d/sdk/Actor/Player/Player.hpp"
#include <algorithm> // For std::max, std::min
#include <cmath>     // For sqrtf, powf
#include <string>
#include <vector>

namespace zr {

class ESPModule : public Module {
private:
  eventpp::CallbackList<void()>::Handle espRenderCallbackHandle;

  // Configurable options (serialized)
  bool showPlayerNames = true;
  bool showPlayerBoxes = true;
  bool showPlayerHealth = true;
  bool showPlayerDistance = true;
  bool showPlayerClassName = true;
  // 是否显示“生命值/生命上限”文本
  bool showPlayerHealthText = true;
  float maxESPDistance = 200.0f;
  ImVec4 enemyBoxColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
  ImVec4 enemyNameColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
  ImVec4 enemyClassNameColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
  ImVec4 enemyDistanceColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
  ImVec4 healthBarFgColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
  ImVec4 healthBarBgColor = ImVec4(0.5f, 0.5f, 0.5f, 0.7f);
  // 生命值文本颜色
  ImVec4 healthTextColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

  ESPModule();

public:
  ESPModule(const ESPModule &) = delete;
  ESPModule &operator=(const ESPModule &) = delete;

  static ESPModule *getInstance();

  std::optional<std::string> load() override;
  std::optional<std::string> enable() override;
  std::optional<std::string> disable() override;
  std::optional<std::string> drawGUI() override;
  std::optional<std::string> toJson(nlohmann::json &json) const override;
  std::optional<std::string> fromJson(const nlohmann::json &json) override;

private:
  void renderESP();
};

} // namespace zr