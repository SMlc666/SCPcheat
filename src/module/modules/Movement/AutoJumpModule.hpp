#pragma once
#include "draw/gui/window/windows/ModuleWindowType.hpp"
#include "module/module.hpp"
#include "nlohmann/json_fwd.hpp"
#include "u3d/sdk/PlayerUpdate.hpp"

namespace zr {
class AutoJumpModule : public Module {
private:
  PlayerUpdateCallbackList::Handle playerUpdateHandle;

  AutoJumpModule() : Module("AutoJumpModule", ModuleWindowType::MOVEMENT) {}

public:
  static AutoJumpModule *getInstance();

  // Module lifecycle methods
  std::optional<std::string> load() override;
  std::optional<std::string> enable() override;
  std::optional<std::string> disable() override;
};
} // namespace zr