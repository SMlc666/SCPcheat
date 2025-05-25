#pragma once

#include <imgui.h>
#include <string>

namespace zr {

class Window {
public:
  std::string title;
  bool visible;

  Window(const std::string &window_title, bool is_visible = true)
      : title(window_title), visible(is_visible) {}

  virtual ~Window() = default;

  virtual void OnRender() = 0;

  virtual void Render() {
    if (!visible) {
      return;
    }

    if (ImGui::Begin(title.c_str(), &visible)) {
      OnRender();
    }
    ImGui::End();
  }

  void Show() { visible = true; }

  void Hide() { visible = false; }

  bool IsVisible() const { return visible; }

  void SetTitle(const std::string &new_title) { title = new_title; }

  const std::string &GetTitle() const { return title; }
};

} // namespace zr