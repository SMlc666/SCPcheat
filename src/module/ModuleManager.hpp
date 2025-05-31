#pragma once
#include "Module.hpp"
#include <functional>
#include <unordered_map>
#include <string_view>
#include <vector>

namespace zr {

class ModuleManager {
private:
    std::unordered_map<std::string, Module*> mModuleList;
    ModuleManager() = default;

public:
    // 禁用拷贝构造和赋值
    ModuleManager(const ModuleManager&) = delete;
    ModuleManager& operator=(const ModuleManager&) = delete;
    
    // 单例访问点
    static ModuleManager& getInstance();
    
    // 遍历所有模块 (callback返回false可终止遍历)
    void forEachModule(std::function<bool(Module*)> callback);
    
    // 按名称获取模块
    Module* getModuleByName(std::string_view moduleName);
    
    // 添加模块 (显式指定名称)
    bool addModule(std::string_view name, Module* module);
    
    // 添加模块 (自动获取名称)
    bool addModule(Module* module);
    
    // 从全局注册表加载所有模块
    void registerModulesFromRegistry();
    
    // 清空所有模块（测试使用）
    void clearModules();
};

} // namespace zr