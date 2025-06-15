#include "log.hpp"
#include "spdlog/logger.h"
#include "spdlog/sinks/stdout_sinks.h"

#include <memory>
#include <mutex>
#include <spdlog/sinks/ringbuffer_sink.h>
#include <unordered_map>

static std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> loggers;
static std::shared_ptr<spdlog::sinks::ringbuffer_sink<std::mutex>> sink =
    std::make_shared<spdlog::sinks::ringbuffer_sink<std::mutex>>(3000);

namespace zr {

std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> &
LogInstance::getLoggers() {
  return loggers;
}

spdlog::logger &LogInstance::getMainLogger() {
  static std::once_flag flag;
  spdlog::logger *logger = nullptr;
  logger = &LogInstance::getOrCreateLogger("main");
  return *logger;
}
spdlog::logger &LogInstance::getOrCreateLogger(const std::string &name) {
  if (loggers.find(name) == loggers.end()) {
    std::shared_ptr<spdlog::logger> logger =
        std::make_shared<spdlog::logger>(name);
    logger->sinks().clear();
    logger->sinks().push_back(sink);
    logger->set_level(spdlog::level::trace);
    logger->set_pattern("%Y-%m-%d %H:%M:%S.%e [%t] [%l] %v");
    loggers.emplace(name, logger);
    return *logger;
  }
  return *loggers[name];
}
std::shared_ptr<spdlog::sinks::ringbuffer_sink<std::mutex>>
LogInstance::getGlobalSink() {
  return sink;
}
} // namespace zr