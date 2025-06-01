#pragma once

#include "draw/gui/window/windows/ModuleWindowType.hpp"
#include <nlohmann/json.hpp>
#include <optional>
#include <spdlog/logger.h>
#include <string>

namespace zr {
class Module {
private:
  bool misEnabled = false;
  bool misLoaded = false;
  std::string mModuleName;
  ModuleWindowType mModuleWindowType;

private:
  virtual std::optional<std::string> enable() = 0;
  virtual std::optional<std::string> disable() = 0;
  virtual std::optional<std::string> load() { return std::nullopt; };
  virtual std::optional<std::string> drawGUI() { return std::nullopt; };
  virtual std::optional<std::string> toJson(nlohmann::json &json) const {
    return std::nullopt;
  };
  virtual std::optional<std::string> fromJson(const nlohmann::json &json) {
    return std::nullopt;
  }

public:
  Module(std::string_view moduleName, ModuleWindowType moduleWindowType)
      : mModuleName(moduleName), mModuleWindowType(moduleWindowType) {}
  bool isEnabled() const { return misEnabled; }
  bool isLoaded() const { return misLoaded; }
  ModuleWindowType getWindowType() const { return mModuleWindowType; }
  std::string getName() const { return mModuleName; }
  std::optional<std::string> enableModule();
  std::optional<std::string> disableModule();
  std::optional<std::string> loadModule();
  std::optional<std::string> drawGUIModule();
  std::optional<std::string> toJsonModule(nlohmann::json &json) const;
  std::optional<std::string> fromJsonModule(const nlohmann::json &json);
  spdlog::logger &getLogger() const;
};
} // namespace zr