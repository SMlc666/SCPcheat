#include "ModuleSetup.hpp"
#include "module/ModuleManager.hpp"

void zr::setupModules() {
  zr::ModuleManager::getInstance().registerModulesFromRegistry();
  ModuleManager::getInstance().forEachModule([](Module *module) -> bool {
    std::optional<std::string> result = module->loadModule();
    if (result.has_value()) {
      module->getLogger().error("Module {} failed to load: {}",
                                module->getName(), result.value());
    } else {
      module->getLogger().info("Module {} loaded successfully",
                               module->getName());
    }
    return true;
  });
}