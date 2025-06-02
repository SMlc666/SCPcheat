#pragma once
#include <filesystem>

namespace zr {
class ConfigHelper {
public:
  static void loadConfigFromFile(std::filesystem::path &configFilePath);
  static void saveConfigToFile(std::filesystem::path &configFilePath);
  static std::filesystem::path getConfigFilePath();
};
void setupConfig();
} // namespace zr