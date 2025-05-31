#include "DebugWindow.hpp"
#include "IL2CPPResolver/il2cpp_resolver.hpp"
#include "draw/gui/window/WindowManager.hpp"
#include "imgui.h"
#include "log/log.hpp"

namespace zr {
DebugWindow *DebugWindow::GetInstance() {
  static DebugWindow instance;
  return &instance;
}

void DebugWindow::OnRender() {
  if (ImGui::Button("Get Player::Update() pointer")) {
    void *PlayerUpdatePtr =
        IL2CPP::Class::Utils::GetMethodPointer("Player", "Update", 0);
    LogInstance::getMainLogger().info(
        "Player::Update() pointer: {:p}",
        reinterpret_cast<void *>(PlayerUpdatePtr));
  }
  ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
}
REGISTER_WINDOW(DebugWindow);
} // namespace zr