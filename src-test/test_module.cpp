#include "module/ModuleManager.hpp"
#include "module/ModuleRegistrar.hpp" // 包含注册宏头文件
#include "module/Module.hpp"
#include <gtest/gtest.h>
#include <string>
#include <vector>

using namespace zr;

// Mock Module for testing
class MockModule : public Module {
public:
  MockModule(std::string_view name, ModuleWindowType type = ModuleWindowType::NONE)
      : Module(name, type) {}

  // Track method calls
  int enableCount = 0;
  int disableCount = 0;
  int loadCount = 0;

protected:
  std::optional<std::string> enable() override {
    enableCount++;
    return std::nullopt;
  }

  std::optional<std::string> disable() override {
    disableCount++;
    return std::nullopt;
  }

  std::optional<std::string> load() override {
    loadCount++;
    return std::nullopt;
  }
};

// 测试专用模块类
class RegistryTestModule : public MockModule {
public:
  static RegistryTestModule *getInstance() {
    static RegistryTestModule instance;
    return &instance;
  }

private:
  RegistryTestModule()
      : MockModule("RegistryTestModule", ModuleWindowType::NONE) {}
};

// 使用宏注册测试模块（传递函数调用结果）
REGISTER_MODULE(RegistryTestModule, RegistryTestModule::getInstance());

// Module tests
TEST(ModuleTest, InitialState) {
  MockModule module("test_module", ModuleWindowType::NONE);
  EXPECT_FALSE(module.isEnabled());
  EXPECT_FALSE(module.isLoaded());
  EXPECT_EQ(module.getName(), "test_module");
}

TEST(ModuleTest, LoadModule) {
  MockModule module("test_module", ModuleWindowType::NONE);
  auto result = module.loadModule();
  EXPECT_FALSE(result.has_value());
  EXPECT_TRUE(module.isLoaded());
  EXPECT_EQ(module.loadCount, 1);
}

TEST(ModuleTest, EnableDisableModule) {
  MockModule module("test_module", ModuleWindowType::NONE);
  module.loadModule();

  auto enableResult = module.enableModule();
  EXPECT_FALSE(enableResult.has_value());
  EXPECT_TRUE(module.isEnabled());
  EXPECT_EQ(module.enableCount, 1);

  auto disableResult = module.disableModule();
  EXPECT_FALSE(disableResult.has_value());
  EXPECT_FALSE(module.isEnabled());
  EXPECT_EQ(module.disableCount, 1);
}

TEST(ModuleTest, EnableBeforeLoadFails) {
  MockModule module("test_module", ModuleWindowType::NONE);
  auto result = module.enableModule();
  EXPECT_TRUE(result.has_value());
  EXPECT_EQ(result.value(), "Module not loaded");
  EXPECT_FALSE(module.isEnabled());
}

TEST(ModuleTest, DisableBeforeLoadFails) {
  MockModule module("test_module", ModuleWindowType::NONE);
  auto result = module.disableModule();
  EXPECT_TRUE(result.has_value());
  EXPECT_EQ(result.value(), "Module not loaded");
  EXPECT_FALSE(module.isEnabled());
}

// ModuleManager tests
TEST(ModuleManagerTest, SingletonInstance) {
  ModuleManager &instance1 = ModuleManager::getInstance();
  ModuleManager &instance2 = ModuleManager::getInstance();
  EXPECT_EQ(&instance1, &instance2);
}

TEST(ModuleManagerTest, AddAndGetModule) {
  ModuleManager &manager = ModuleManager::getInstance();
  MockModule *module = new MockModule("test_module", ModuleWindowType::NONE);

  // Add by explicit name
  EXPECT_TRUE(manager.addModule("test_module", module));
  EXPECT_EQ(manager.getModuleByName("test_module"), module);

  // Add duplicate should fail
  EXPECT_FALSE(manager.addModule("test_module", new MockModule("duplicate", ModuleWindowType::NONE)));
}

TEST(ModuleManagerTest, AddModuleAutoName) {
  ModuleManager &manager = ModuleManager::getInstance();
  MockModule *module = new MockModule("auto_name_module", ModuleWindowType::NONE);

  EXPECT_TRUE(manager.addModule(module));
  EXPECT_EQ(manager.getModuleByName("auto_name_module"), module);
}


// 测试注册宏功能
TEST(ModuleManagerTest, RegisterFromRegistry) {
  ModuleManager &manager = ModuleManager::getInstance();

  // 注册前模块不应存在
  EXPECT_EQ(manager.getModuleByName("RegistryTestModule"), nullptr);

  // 从注册表加载模块
  manager.registerModulesFromRegistry();

  // 验证模块已正确加载
  Module *module = manager.getModuleByName("RegistryTestModule");
  ASSERT_NE(module, nullptr);
  EXPECT_EQ(module->getName(), "RegistryTestModule");

  // 测试模块基本功能
  EXPECT_FALSE(module->isEnabled());
  EXPECT_FALSE(module->isLoaded());

  auto loadResult = module->loadModule();
  EXPECT_FALSE(loadResult.has_value());
  EXPECT_TRUE(module->isLoaded());

  auto enableResult = module->enableModule();
  EXPECT_FALSE(enableResult.has_value());
  EXPECT_TRUE(module->isEnabled());
}