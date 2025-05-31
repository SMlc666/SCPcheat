#include "log/log.hpp"
#include <gtest/gtest.h>
#include <memory>
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

TEST(LogTest, MainLoggerUsesRingBufferSink) {
  auto& mainLogger = LogInstance::getMainLogger();
  auto globalSink = LogInstance::getGlobalSink();
  
  // 获取主日志记录器的 sink 列表
  auto sinks = mainLogger.sinks();
  
  // 检查是否有 ringbuffer_sink
  bool foundRingBufferSink = false;
  for (auto& sink : sinks) {
    if (std::dynamic_pointer_cast<spdlog::sinks::ringbuffer_sink<std::mutex>>(sink)) {
      foundRingBufferSink = true;
      
      // 验证是否是全局 sink
      EXPECT_EQ(sink, globalSink);
      break;
    }
  }
  
  EXPECT_TRUE(foundRingBufferSink);
}

TEST(LogTest, MainLoggerOutputToRingBuffer) {
  auto& mainLogger = LogInstance::getMainLogger();
  auto globalSink = LogInstance::getGlobalSink();

 
  const std::string test_message = "Test ringbuffer output";
  mainLogger.info(test_message);

  // 从ringbuffer中获取最近的日志
  auto logs = globalSink->last_formatted(1);
  ASSERT_FALSE(logs.empty()) << "No logs in ringbuffer after info message";
  EXPECT_NE(logs[0].find(test_message), std::string::npos)
      << "Logged message not found in ringbuffer";
}

