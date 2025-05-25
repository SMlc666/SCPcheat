#include "LogWindow.hpp"
#include "draw/gui/window/WindowManager.hpp"
#include "imgui.h"

namespace zr {
LogWindow *LogWindow::GetInstance() {
  static LogWindow instance;
  return &instance;
}
void LogWindow::OnRender() { ImGui::Text("Log Window"); }
REGISTER_WINDOW(LogWindow);
} // namespace zr