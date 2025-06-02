#pragma once

#include "eventpp/callbacklist.h"
namespace zr {
extern eventpp::CallbackList<void()> ImGuiCallback;
void setupImGui();
} // namespace zr