#include "module.hpp"
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
