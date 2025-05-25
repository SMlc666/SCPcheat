#pragma once

#include "draw/gui/window/window.hpp"

namespace zr {
class LogWindow : public Window {
public:
  static LogWindow *GetInstance();
  void OnRender() override;
  LogWindow() : Window("Log", false) {}
};
} // namespace zr