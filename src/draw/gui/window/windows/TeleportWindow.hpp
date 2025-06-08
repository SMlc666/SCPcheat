#pragma once

#include "draw/gui/window/window.hpp"
namespace zr {
class TeleportWindow : public Window {
public:
  static TeleportWindow *GetInstance();
  void OnRender() override;
  TeleportWindow() : Window("Teleport", true) {}
};
} // namespace zr