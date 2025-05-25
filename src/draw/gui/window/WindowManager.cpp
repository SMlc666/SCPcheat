#include "WindowManager.hpp"

namespace zr {


std::vector<Window *> WindowManager::windows;

void WindowManager::RegisterWindow(Window *window) {
  if (!window)
    return;
  
  for (auto *w : windows) {
    if (w == window)
      return;
  }
  windows.push_back(window);
}

void WindowManager::UnregisterWindow(Window *window) {
  if (!window)
    return;
  for (auto it = windows.begin(); it != windows.end(); ++it) {
    if (*it == window) {
      windows.erase(it);
      return;
    }
  }
}

void WindowManager::RenderAll() {
  for (auto *w : windows) {
    if (w && w->IsVisible()) {
      w->Render();
    }
  }
}

const std::vector<Window *> &WindowManager::GetWindows() { return windows; }

} // namespace zr