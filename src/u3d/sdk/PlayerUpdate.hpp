#pragma once
#include "IL2CPPResolver/API/Class.hpp"
#include "u3d/sdk/Actor/Player/Player.hpp"
#include <eventpp/callbacklist.h>

namespace zr {
void setupU3DPlayerUpdate();

// 使用eventpp回调列表类型
using PlayerUpdateCallbackList = eventpp::CallbackList<void(Player *)>;

// 直接暴露回调列表对象供外部使用
extern PlayerUpdateCallbackList playerUpdateCallbacks;

extern std::vector<std::function<void(Player *)>> PlayerUpdateOnceCallbackList;
} // namespace zr