#include "PlayerUpdate.hpp"
#include "IL2CPPResolver/il2cpp_resolver.hpp"
#include "safetyhook.hpp"
#include <atomic>
#include <map>
#include <mutex>

safetyhook::InlineHook Player_Update_hook;

namespace zr {
// 回调容器
static std::map<uint64_t, PlayerUpdateCallback> s_callbacks;
static std::mutex s_callbackMutex;
static std::atomic<uint64_t> s_nextHandle{1};

uint64_t addPlayerUpdateCallback(PlayerUpdateCallback callback) {
  std::lock_guard<std::mutex> lock(s_callbackMutex);
  uint64_t handle = s_nextHandle++;
  s_callbacks[handle] = callback;
  return handle;
}

void removePlayerUpdateCallback(uint64_t handle) {
  std::lock_guard<std::mutex> lock(s_callbackMutex);
  s_callbacks.erase(handle);
}

static void PlayerUpdate_Proxy(Unity::CGameObject *m_pGameObject) {
  // 调用原始函数
  Player_Update_hook.call(m_pGameObject);

  // 执行所有注册的回调
  std::lock_guard<std::mutex> lock(s_callbackMutex);
  for (auto &[handle, callback] : s_callbacks) {
    if (callback) {
      callback(m_pGameObject);
    }
  }
}

void setupU3DPlayerUpdate() {
  void *Player_UpdateAddr =
      IL2CPP::Class::Utils::GetMethodPointer("Player", "Update", 1);
  Player_Update_hook =
      safetyhook::create_inline(Player_UpdateAddr, PlayerUpdate_Proxy);
}
} // namespace zr