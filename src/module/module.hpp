#pragma once

#include <optional>
#include <string>
namespace zr {
class Module {
private:
  bool misEnabled = false;
  bool misLoaded = false;
  std::string mModuleName;

private:
  virtual std::optional<std::string> enable() = 0;
  virtual std::optional<std::string> disable() = 0;
  virtual std::optional<std::string> load() = 0;

public:
  Module(std::string_view moduleName) : mModuleName(moduleName) {}
  bool isEnabled() const { return misEnabled; }
  bool isLoaded() const { return misLoaded; }
  std::string_view getName() const { return mModuleName; }
  std::optional<std::string> enableModule();
  std::optional<std::string> disableModule();
  std::optional<std::string> loadModule();
};
} // namespace zr