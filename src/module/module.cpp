#include "module.hpp"
#include "log/log.hpp"
#include <cstddef>
#include <optional>
#include <string>

std::optional<std::string> zr::Module::loadModule() {
  if (misLoaded) {
    return std::nullopt;
  }
  misLoaded = true;
  return load();
}
std::optional<std::string> zr::Module::disableModule() {
  if (!misLoaded) {
    return "Module not loaded";
  }
  if (!misEnabled) {
    return std::nullopt;
  }
  misEnabled = false;
  return disable();
}
std::optional<std::string> zr::Module::enableModule() {
  if (!misLoaded) {
    return "Module not loaded";
  }
  if (misEnabled) {
    return std::nullopt;
  }
  misEnabled = true;
  return enable();
}
std::optional<std::string> zr::Module::drawGUIModule() {
  if (!misLoaded) {
    return "Module not loaded";
  }
  return drawGUI();
}
std::optional<std::string>
zr::Module::fromJsonModule(const nlohmann::json &json) {
  return fromJson(json);
}
std::optional<std::string>
zr::Module::toJsonModule(nlohmann::json &json) const {
  return toJson(json);
}
spdlog::logger &zr::Module::getLogger() const {
  return zr::LogInstance::getOrCreateLogger(mModuleName);
}
