#include "SpeedModule.hpp"
#include "eventpp/callbacklist.h"
#include "imgui.h"
#include "module/ModuleRegistrar.hpp"
#include "u3d/sdk/PlayerUpdate.hpp"

std::optional<std::string> zr::SpeedModule::load() { return std::nullopt; }
std::optional<std::string> zr::SpeedModule::enable() {
  playerUpdateHandle = playerUpdateCallbacks.append([this](Player *player) {
    if (player) {
      player->setSpeed(walkSpeed, runSpeed);
    }
  });
  return std::nullopt;
}
std::optional<std::string> zr::SpeedModule::disable() {
  playerUpdateCallbacks.remove(playerUpdateHandle);
  return std::nullopt;
}

std::optional<std::string> zr::SpeedModule::drawGUI() {
  ImGui::SliderFloat("walkSpeed", &walkSpeed, 1, 30);
  ImGui::SliderFloat("runSpeed", &runSpeed, 1, 30);
  return std::nullopt;
}
zr::SpeedModule *zr::SpeedModule::getInstance() {
  static SpeedModule instance;
  return &instance;
}

std::optional<std::string> zr::SpeedModule::toJson(nlohmann::json &json) const {
  json["walkSpeed"] = walkSpeed;
  json["runSpeed"] = runSpeed;
  return std::nullopt;
}
std::optional<std::string>
zr::SpeedModule::fromJson(const nlohmann::json &json) {
  if (json.contains("walkSpeed") && json["walkSpeed"].is_number()) {
    walkSpeed = json["walkSpeed"].get<float>();
  } else {
    return "Missing 'walkSpeed' in JSON";
  }
  if (json.contains("runSpeed") && json["runSpeed"].is_number()) {
    runSpeed = json["runSpeed"].get<float>();
  } else {
    return "Missing 'runSpeed' in JSON";
  }
  return std::nullopt;
}
namespace zr {
REGISTER_MODULE(SpeedModule, SpeedModule::getInstance());
}