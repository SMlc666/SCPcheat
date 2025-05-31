#pragma once

#include "draw/gui/window/window.hpp"
#include <string>

namespace zr {
class LogWindow : public Window {
public:
  static LogWindow *GetInstance();
  void OnRender() override;
  LogWindow() : Window("Log", true) {
    // 默认显示所有日志等级
    for (int i = 0; i < 6; i++) {
      m_LevelFilters[i] = true;
    }
  }

private:
  bool m_LevelFilters[6];   // trace, debug, info, warn, error, critical
  std::string m_FilterText; // 关键词过滤
};
} // namespace zr