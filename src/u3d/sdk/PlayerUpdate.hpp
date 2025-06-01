#pragma once
#include "IL2CPPResolver/IL2CPP_Resolver.hpp"
#include <eventpp/callbacklist.h>

namespace zr {
void setupU3DPlayerUpdate();

// 使用eventpp回调列表类型
using PlayerUpdateCallbackList = eventpp::CallbackList<void(IL2CPP::CClass *)>;

// 直接暴露回调列表对象供外部使用
extern PlayerUpdateCallbackList playerUpdateCallbacks;
} // namespace zr