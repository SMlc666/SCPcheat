#include "config/config.hpp"
#include <filesystem>
#include <gtest/gtest.h>


// Mock or stub classes for ModuleManager and Module may be needed for full
// tests

namespace {

class ConfigHelperTest : public ::testing::Test {
protected:
  std::filesystem::path testConfigPath;

  void SetUp() override {
    // 设置测试配置文件路径
    testConfigPath =
        std::filesystem::temp_directory_path() / "test_config.json";
  }

  void TearDown() override {
    // 测试结束后删除测试文件
    if (std::filesystem::exists(testConfigPath)) {
      std::filesystem::remove(testConfigPath);
    }
  }
};

TEST_F(ConfigHelperTest, LoadConfigFromFile_FileNotExist) {
  // 测试加载不存在的文件，不应崩溃
  std::filesystem::path nonExistPath =
      std::filesystem::temp_directory_path() / "non_exist_config.json";
  zr::ConfigHelper::loadConfigFromFile(nonExistPath);
  // 这里主要验证无异常，日志输出不易测试
  SUCCEED();
}

TEST_F(ConfigHelperTest, SaveConfigToFile_FileWrite) {
  // 测试保存配置文件，主要验证文件是否创建
  zr::ConfigHelper::saveConfigToFile(testConfigPath);
  EXPECT_TRUE(std::filesystem::exists(testConfigPath));
}

TEST_F(ConfigHelperTest, SetupConfig_CreatesFileIfNotExist) {
  if (std::filesystem::exists(testConfigPath)) {
    std::filesystem::remove(testConfigPath);
  }
  // 由于setupConfig内部路径固定，无法直接传入测试路径，测试有限
  // 这里只调用函数，确保不崩溃
  zr::setupConfig();
  SUCCEED();
}

} // namespace