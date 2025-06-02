#pragma once

#include "draw/gui/window/window.hpp"
namespace zr {
class ModuleWindow : public Window {
public:
  static ModuleWindow *GetInstance();
  ModuleWindow() : Window("ModuleManager", true) {}
  void OnRender() override;
};
} // namespace zr