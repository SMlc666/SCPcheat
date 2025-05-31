#pragma once
#include "Module.hpp"
#include <functional>
#include <vector>

namespace zr {
namespace detail {
// 模块工厂函数类型
using ModuleFactory = std::function<Module *()>;

// 获取全局模块注册表（线程安全）
inline std::vector<ModuleFactory> &GetGlobalModuleRegistry() {
  static std::vector<ModuleFactory> registry;
  return registry;
}

// 自动注册辅助类
struct ModuleRegistrar {
  ModuleRegistrar(ModuleFactory factory) {
    GetGlobalModuleRegistry().push_back(factory);
  }
};
} // namespace detail
} // namespace zr

// 模块注册宏
// ModuleClass: 模块类名
// InstanceGetter: 获取模块实例的表达式（如&MyModule::getInstance）
#define REGISTER_MODULE(ModuleClass, InstanceGetter)                           \
  namespace { /* 匿名命名空间避免符号冲突 */                                   \
  auto zr_module_factory_##ModuleClass = []() -> zr::Module * {                \
    return static_cast<zr::Module *>(InstanceGetter);                          \
  };                                                                           \
  static zr::detail::ModuleRegistrar                                           \
      zr_registrar_##ModuleClass(zr_module_factory_##ModuleClass);             \
  }