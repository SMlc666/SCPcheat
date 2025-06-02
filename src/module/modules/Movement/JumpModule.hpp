#pragma once
#include "draw/gui/window/windows/ModuleWindowType.hpp"
#include "module/module.hpp"
#include "nlohmann/json_fwd.hpp"
#include "u3d/sdk/PlayerUpdate.hpp"

namespace zr {
class JumpModule : public Module {
private:
  float jumpMultiplier = 1.0f;
  PlayerUpdateCallbackList::Handle playerUpdateHandle;

  JumpModule() : Module("JumpModule", ModuleWindowType::MOVEMENT) {}

public:
  static JumpModule *getInstance();

  // Module lifecycle methods
  std::optional<std::string> load() override;
  std::optional<std::string> enable() override;
  std::optional<std::string> disable() override;
  std::optional<std::string> drawGUI() override;
  std::optional<std::string> toJson(nlohmann::json &json) const override;
  std::optional<std::string> fromJson(const nlohmann::json &json) override;
};
} // namespace zr