#include "config.hpp"
#include "log/log.hpp"
#include "module/ModuleManager.hpp"
#include "module/module.hpp"
#include "utils/FileUtils.hpp"
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
static std::filesystem::path configFilePath;
namespace zr {
std::filesystem::path ConfigHelper::getConfigFilePath() {
  return configFilePath;
}
void ConfigHelper::loadConfigFromFile(std::filesystem::path &configFilePath) {
  std::ifstream ifs(configFilePath);
  if (!ifs.is_open()) {
    zr::LogInstance::getMainLogger().warn(
        "ConfigHelper: Failed to open config file for reading: {}",
        configFilePath.string());
    return;
  }

  nlohmann::json allConfigsJson;
  try {
    ifs >> allConfigsJson;
  } catch (const std::exception &e) {
    zr::LogInstance::getMainLogger().error(
        "ConfigHelper: Failed to parse JSON config file {}: {}",
        configFilePath.string(), e.what());
    return;
  }

  auto &moduleManager = ModuleManager::getInstance();
  moduleManager.forEachModule([&allConfigsJson](Module *module) {
    std::string moduleName = module->getName();

    nlohmann::json originJson;
    auto originErr = module->toJsonModule(originJson);
    if (originErr.has_value()) {
      zr::LogInstance::getMainLogger().warn(
          "ConfigHelper: Failed to get original config from module {}: {}",
          moduleName, originErr.value());
    }

    if (allConfigsJson.contains(moduleName)) {
      const auto &moduleConfigJson = allConfigsJson[moduleName];
      auto err = module->fromJsonModule(moduleConfigJson);
      if (err.has_value()) {
        auto &logger = module->getLogger();
        logger.error("ConfigHelper: Failed to load config for module {}: {}",
                     moduleName, err.value());
        // Rollback to original config
        auto rollbackErr = module->fromJsonModule(originJson);
        if (rollbackErr.has_value()) {
          logger.error(
              "ConfigHelper: Failed to rollback config for module {}: {}",
              moduleName, rollbackErr.value());
        }
      }
    } else {
      zr::LogInstance::getMainLogger().warn(
          "ConfigHelper: No config found for module {} in config file",
          moduleName);
    }
    return true; // continue iteration
  });
}

void ConfigHelper::saveConfigToFile(std::filesystem::path &configFilePath) {
  nlohmann::json allConfigsJson;

  auto &moduleManager = ModuleManager::getInstance();
  moduleManager.forEachModule([&allConfigsJson](Module *module) {
    std::string moduleName = module->getName();
    nlohmann::json moduleConfigJson;
    auto err = module->toJsonModule(moduleConfigJson);
    if (err.has_value()) {
      zr::LogInstance::getMainLogger().warn(
          "ConfigHelper: Failed to get config from module {}: {}", moduleName,
          err.value());
      return true; // continue iteration
    }
    allConfigsJson[moduleName] = moduleConfigJson;
    return true; // continue iteration
  });

  std::ofstream ofs(configFilePath);
  if (!ofs.is_open()) {
    zr::LogInstance::getMainLogger().error(
        "ConfigHelper: Failed to open config file for writing: {}",
        configFilePath.string());
    return;
  }

  try {
    ofs << allConfigsJson.dump(4);
  } catch (const std::exception &e) {
    zr::LogInstance::getMainLogger().error(
        "ConfigHelper: Failed to write JSON config file {}: {}",
        configFilePath.string(), e.what());
  }
}
void setupConfig() {
  static std::string configFileName = "config.json";
  configFilePath = zr::get_process_executable_path();
  std::filesystem::path configDir = configFilePath.parent_path();
  configFilePath = configDir / configFileName;
  if (!std::filesystem::exists(configFilePath)) {
    zr::LogInstance::getMainLogger().info(
        "Config file not found, creating new one at: {}",
        configFilePath.string());
    std::ofstream ofs(configFilePath);
    if (!ofs.is_open()) {
      zr::LogInstance::getMainLogger().error(
          "Failed to create config file at: {}", configFilePath.string());
      return;
    }
    ofs << "{}"; // Create an empty JSON object
    ofs.close();
  }
  zr::ConfigHelper::loadConfigFromFile(configFilePath);
}
} // namespace zr