#pragma once
#include "module/module.hpp"
#include "u3d/sdk/PlayerUpdate.hpp"
#include <optional>
#include <string>

namespace zr {
class DoorModule : public Module {
public:
  bool always_open = false;
  bool always_break = false;
  bool usingClientBreak = false;
  PlayerUpdateCallbackList::Handle updatehandle;
  DoorModule() : Module("DoorModule", ModuleWindowType::MISC) {}
  static DoorModule *getInstance();
  std::optional<std::string> load() override;
  std::optional<std::string> enable() override;
  std::optional<std::string> disable() override;
  std::optional<std::string> toJson(nlohmann::json &j) const override;
  std::optional<std::string> fromJson(const nlohmann::json &j) override;
  std::optional<std::string> drawGUI() override;
};
} // namespace zr