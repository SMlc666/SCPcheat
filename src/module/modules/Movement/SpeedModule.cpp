#include "SpeedModule.hpp"
#include "eventpp/callbacklist.h"
#include "imgui.h"
#include "module/ModuleRegistrar.hpp"
#include "u3d/sdk/PlayerUpdate.hpp"

std::optional<std::string> zr::SpeedModule::load() { return std::nullopt; }
std::optional<std::string> zr::SpeedModule::enable() {
  playerUpdateHandle =
      playerUpdateCallbacks.append([this](IL2CPP::CClass *player) {
        if (player) {
          
          player->CallMethod<void>("SetSpeed", walkSpeed, runSpeed);
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
namespace zr {
REGISTER_MODULE(SpeedModule, SpeedModule::getInstance());
}