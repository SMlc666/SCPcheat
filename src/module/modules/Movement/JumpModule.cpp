#include "JumpModule.hpp"
#include "eventpp/callbacklist.h"
#include "imgui.h"
#include "module/ModuleRegistrar.hpp"
#include "u3d/sdk/Actor/Player/Player.hpp"
#include <optional>

using namespace zr;

std::optional<std::string> JumpModule::load() { return std::nullopt; }

std::optional<std::string> JumpModule::enable() {
  playerUpdateHandle = playerUpdateCallbacks.append([this](Player *player) {
    if (player) {
      // 设置跳跃高度 = 5.0 * 当前倍数
      player->setJumpPower(5.0f * jumpMultiplier);
    }
  });
  return std::nullopt;
}

std::optional<std::string> JumpModule::disable() {
  playerUpdateCallbacks.remove(playerUpdateHandle);
  return std::nullopt;
}

std::optional<std::string> JumpModule::drawGUI() {
  ImGui::SliderFloat("跳跃倍数", &jumpMultiplier, 0.01f, 10.0f, "%.1f");
  return std::nullopt;
}

JumpModule *JumpModule::getInstance() {
  static JumpModule instance;
  return &instance;
}
std::optional<std::string> JumpModule::toJson(nlohmann::json &json) const {
  json["jumpMultiplier"] = jumpMultiplier;
  return std::nullopt;
}
std::optional<std::string> JumpModule::fromJson(const nlohmann::json &json) {
  if (json.contains("jumpMultiplier") && json["jumpMultiplier"].is_number()) {
    jumpMultiplier = json["jumpMultiplier"].get<float>();
  } else {
    return "Missing 'jumpMultiplier' in JSON";
  }
  return std::nullopt;
}
// 模块注册
REGISTER_MODULE(JumpModule, JumpModule::getInstance());