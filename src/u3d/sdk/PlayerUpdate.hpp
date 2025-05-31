#pragma once
#include "IL2CPPResolver/Unity/Includes.hpp"
#include <cstdint>
#include <functional>


namespace zr {
void setupU3DPlayerUpdate();

// 回调函数类型定义
using PlayerUpdateCallback = std::function<void(Unity::CGameObject *)>;

// 添加回调，返回句柄
uint64_t addPlayerUpdateCallback(PlayerUpdateCallback callback);

// 移除回调
void removePlayerUpdateCallback(uint64_t handle);
} // namespace zr