#pragma once
#include "draw/gui/window/windows/ModuleWindowType.hpp"
#include "module/module.hpp"
#include "u3d/sdk/PlayerUpdate.hpp"
namespace zr {
class SpeedModule : public Module {
private:
  float walkSpeed = 1.0f;
  float runSpeed = 1.0f;
  PlayerUpdateCallbackList::Handle playerUpdateHandle;
  SpeedModule() : Module("SpeedModule", ModuleWindowType::MOVEMENT) {}
  std::optional<std::string> load() override;
  std::optional<std::string> enable() override;
  std::optional<std::string> disable() override;
  std::optional<std::string> drawGUI() override;

public:
  static SpeedModule *getInstance();
};
} // namespace zr