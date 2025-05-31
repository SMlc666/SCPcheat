#pragma once
#include "draw/gui/window/window.hpp"
#include "draw/gui/window/WindowManager.hpp"
#include <algorithm> // 用于std::remove

namespace zr {
class ManagerWindow : public Window {
public:
    static ManagerWindow* GetInstance();
    void OnRender() override;
    
    ManagerWindow() : Window("Window Manager", true) {}

private:
    bool m_ShowAll = true;
    bool m_HideAll = false;
};
} // namespace zr
