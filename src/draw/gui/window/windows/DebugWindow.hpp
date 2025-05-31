#pragma once

#include "draw/gui/window/window.hpp"
#include <string>

namespace zr {
class DebugWindow : public Window {
public:
  static DebugWindow* GetInstance();
  void OnRender() override;
  
  DebugWindow() : Window("Debug", true) {}
  
private:
  // 可添加调试专用状态变量
};
} // namespace zr