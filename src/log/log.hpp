#pragma once
#include "spdlog/logger.h"
#include "spdlog/sinks/ringbuffer_sink.h"
#include <memory>
#include <spdlog/spdlog.h>
namespace zr {
class LogInstance {
public:
  static std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> &
  getLoggers();
  static spdlog::logger &getMainLogger();
  static spdlog::logger &getOrCreateLogger(const std::string &name);
  static std::shared_ptr<spdlog::sinks::ringbuffer_sink<std::mutex>>
  getGlobalSink();
};
} // namespace zr