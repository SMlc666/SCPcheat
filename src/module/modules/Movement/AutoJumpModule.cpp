#include "AutoJumpModule.hpp"
#include "module/ModuleRegistrar.hpp"
#include "u3d/sdk/Player/LocalPlayer.hpp"

namespace zr {
AutoJumpModule *AutoJumpModule::getInstance() {
  static AutoJumpModule instance;
  return &instance;
}
std::optional<std::string> AutoJumpModule::load() { return std::nullopt; }
std::optional<std::string> AutoJumpModule::enable() {
  playerUpdateHandle = playerUpdateCallbacks.append([](Player *player) {
    if (player == LocalPlayer::getInstance()) {
      auto control = player->getFirstPersonController();
      if (control) {
        control->setJump(true);
      }
    }
  });
  return std::nullopt;
}
std::optional<std::string> AutoJumpModule::disable() {
  playerUpdateCallbacks.remove(playerUpdateHandle);
  return std::nullopt;
}
REGISTER_MODULE(AutoJumpModule, AutoJumpModule::getInstance());
} // namespace zr