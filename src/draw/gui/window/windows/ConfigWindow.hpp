#pragma once

#include "draw/gui/window/window.hpp"
#include <string>

namespace zr {
class ConfigWindow : public Window {
public:
    static ConfigWindow* GetInstance();
    void OnRender() override;

    ConfigWindow();
};
} // namespace zr