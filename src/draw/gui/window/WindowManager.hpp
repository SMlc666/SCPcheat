#pragma once

#include "window.hpp"
#include <vector>

namespace zr {

class WindowManager {
public:
  static void RegisterWindow(Window *window);

  static void UnregisterWindow(Window *window);

  static void RenderAll();

  static const std::vector<Window *> &GetWindows();

private:
  static std::vector<Window *> windows;
};

#define REGISTER_WINDOW(className) \
  struct className##Register { \
    className##Register() { \
      zr::WindowManager::RegisterWindow(className::GetInstance()); \
    } \
  }; \
  static className##Register global_##className##Register;

} // namespace zr