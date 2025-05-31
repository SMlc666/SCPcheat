#include "ModuleManager.hpp"
#include "ModuleRegistrar.hpp"

namespace zr {

ModuleManager& ModuleManager::getInstance() {
    static ModuleManager instance;
    return instance;
}

void ModuleManager::forEachModule(std::function<bool(Module*)> callback) {
    for (auto& [name, module] : mModuleList) {
        if (!callback(module)) break;
    }
}

Module* ModuleManager::getModuleByName(std::string_view moduleName) {
    auto it = mModuleList.find(std::string(moduleName));
    return (it != mModuleList.end()) ? it->second : nullptr;
}

bool ModuleManager::addModule(std::string_view name, Module* module) {
    if (!module) return false;
    auto [it, inserted] = mModuleList.emplace(std::string(name), module);
    return inserted;
}

bool ModuleManager::addModule(Module* module) {
    return module ? addModule(module->getName(), module) : false;
}

void ModuleManager::clearModules() {
    mModuleList.clear();
}

void ModuleManager::registerModulesFromRegistry() {
    for (auto& factory : detail::GetGlobalModuleRegistry()) {
        if (Module* module = factory()) {
            addModule(module);
        }
    }
}

} // namespace zr