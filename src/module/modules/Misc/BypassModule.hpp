#pragma once

#include "module/module.hpp"
#include <optional>
#include <string>
namespace zr {
class BypassModule : public Module {
public:
  bool isDebug = false;
  bool randomDeviceID = true;
  BypassModule() : Module("BypassModule", ModuleWindowType::MISC) {}
  static BypassModule *getInstance();
  std::optional<std::string> load() override;
  std::optional<std::string> enable() override;
  std::optional<std::string> disable() override;
  std::optional<std::string> toJson(nlohmann::json &j) const override;
  std::optional<std::string> fromJson(const nlohmann::json &j) override;
  std::optional<std::string> drawGUI() override;
};
} // namespace zr