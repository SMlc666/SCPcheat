#include "LogWindow.hpp"
#include "draw/gui/window/WindowManager.hpp"
#include "imgui.h"
#include "log/log.hpp"     // 包含log.hpp以使用LogInstance
#include <algorithm>       // 用于std::transform
#include <cctype>          // 用于std::tolower
#include <spdlog/common.h> // 包含日志等级定义

namespace zr {
LogWindow *LogWindow::GetInstance() {
  static LogWindow instance;
  return &instance;
}

void LogWindow::OnRender() {
  // 获取全局环形缓冲区sink
  auto sink = LogInstance::getGlobalSink();
  if (!sink) {
    ImGui::Text("Failed to get log sink!");
    return;
  }

  // 添加关键词搜索框
  char buffer[256] = {0};
  size_t length = sizeof(buffer) - 1;
  strncpy_s(buffer, sizeof(buffer), m_FilterText.c_str(), length);
  if (ImGui::InputText("Search", buffer, sizeof(buffer))) {
    m_FilterText = buffer;
  }

  // 添加日志等级过滤
  ImGui::Checkbox("Trace", &m_LevelFilters[0]);
  ImGui::SameLine();
  ImGui::Checkbox("Debug", &m_LevelFilters[1]);
  ImGui::SameLine();
  ImGui::Checkbox("Info", &m_LevelFilters[2]);
  ImGui::SameLine();
  ImGui::Checkbox("Warn", &m_LevelFilters[3]);
  ImGui::SameLine();
  ImGui::Checkbox("Error", &m_LevelFilters[4]);
  // 添加测试日志按钮
  if (ImGui::Button("Test Log")) {
    // 获取主日志记录器并记录测试日志
    auto &logger = LogInstance::getMainLogger();
    logger.info("This is a test log message from LogWindow");
  }
  ImGui::SameLine();
  ImGui::Checkbox("Critical", &m_LevelFilters[5]);

  // 设置可滚动区域
  ImGui::BeginChild("LogContent", ImVec2(0, 0), true);

  // 获取并显示日志（应用等级和关键词过滤）
  const auto logs = sink->last_formatted();
  for (const auto &log : logs) {
    // 提取日志等级，获取第二个方括号内的内容
    std::string levelStr;
    bool hasLevel = false;
    size_t firstBracket = log.find('[');
    if (firstBracket != std::string::npos) {
        size_t secondBracket = log.find('[', firstBracket + 1);
        if (secondBracket != std::string::npos) {
            size_t levelStart = secondBracket;
            size_t levelEnd = log.find(']', levelStart);
            if (levelEnd != std::string::npos) {
                levelStr = log.substr(levelStart + 1, levelEnd - levelStart - 1);
                hasLevel = true;
            }
        }
    }

    if (hasLevel) {
      // 检查日志等级是否被选中 (适配spdlog缩写)
      bool showLog = false;
      if (levelStr == "trace" || levelStr == "t") showLog = m_LevelFilters[0];
      else if (levelStr == "debug" || levelStr == "d") showLog = m_LevelFilters[1];
      else if (levelStr == "info" || levelStr == "i") showLog = m_LevelFilters[2];
      else if (levelStr == "warn" || levelStr == "w") showLog = m_LevelFilters[3];
      else if (levelStr == "error" || levelStr == "e") showLog = m_LevelFilters[4];
      else if (levelStr == "critical" || levelStr == "c") showLog = m_LevelFilters[5];

      // 应用关键词过滤
      if (showLog && !m_FilterText.empty()) {
        // 转换为小写进行不区分大小写的搜索
        std::string logLower = log;
        std::transform(logLower.begin(), logLower.end(), logLower.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        std::string filterLower = m_FilterText;
        std::transform(filterLower.begin(), filterLower.end(),
                       filterLower.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        showLog = (logLower.find(filterLower) != std::string::npos);
      }

      if (showLog) {
        ImGui::TextUnformatted(log.c_str());
      }
    } // 结束 hasLevel 条件
  }

  // 自动滚动到底部
  if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
    ImGui::SetScrollHereY(1.0f);
  }

  ImGui::EndChild();
}

REGISTER_WINDOW(LogWindow);
} // namespace zr