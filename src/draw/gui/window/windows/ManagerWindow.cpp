#include "ManagerWindow.hpp"
#include <algorithm> // for std::remove
#include <imgui.h>
#include <vector>


namespace zr {
ManagerWindow *ManagerWindow::GetInstance() {
  static ManagerWindow instance;
  return &instance;
}

void ManagerWindow::OnRender() {
  // 获取所有窗口
  auto &windows = WindowManager::GetWindows();

  // 创建窗口列表的副本（因为GetWindows返回const引用）
  std::vector<Window *> windowList(windows.begin(), windows.end());

  // 移除自身窗口（避免管理自己）
  windowList.erase(std::remove(windowList.begin(), windowList.end(), this),
                   windowList.end());

  // 全部显示/隐藏按钮
  if (ImGui::Button("Show All")) {
    for (auto window : windowList) {
      window->Show();
    }
  }
  ImGui::SameLine();
  if (ImGui::Button("Hide All")) {
    for (auto window : windowList) {
      window->Hide();
    }
  }

  // 窗口列表
  ImGui::Separator();
  for (auto window : windowList) {
    bool visible = window->IsVisible();
    if (ImGui::Checkbox(window->GetTitle().c_str(), &visible)) {
      visible ? window->Show() : window->Hide();
    }
  }
}
// 自动注册
REGISTER_WINDOW(ManagerWindow);
} // namespace zr
