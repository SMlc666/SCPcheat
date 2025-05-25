#include "log/log.hpp"
#include <gtest/gtest.h>
#include <string>

using namespace zr;

TEST(LogTest, CreateAndRetrieveLogger) {
  auto &logger1 = LogInstance::getOrCreateLogger("test_logger");
  auto &logger2 = LogInstance::getOrCreateLogger("test_logger");
  EXPECT_EQ(&logger1, &logger2);
  EXPECT_EQ(logger1.name(), "test_logger");
}

TEST(LogTest, GetMainLoggerSingleton) {
  auto &mainLogger1 = LogInstance::getMainLogger();
  auto &mainLogger2 = LogInstance::getMainLogger();
  EXPECT_EQ(&mainLogger1, &mainLogger2);
  EXPECT_EQ(mainLogger1.name(), "main");
}

TEST(LogTest, GetLoggersContainsCreatedLoggers) {
  LogInstance::getOrCreateLogger("logger1");
  LogInstance::getOrCreateLogger("logger2");
  auto &loggers = LogInstance::getLoggers();
  EXPECT_NE(loggers.find("logger1"), loggers.end());
  EXPECT_NE(loggers.find("logger2"), loggers.end());
}
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}