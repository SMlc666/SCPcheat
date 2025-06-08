#include "PlayerUpdate.hpp"
#include "log/log.hpp"
#include "safetyhook.hpp"

safetyhook::InlineHook Player_Update_hook;

namespace zr {
// 定义回调列表对象
PlayerUpdateCallbackList playerUpdateCallbacks;
std::vector<std::function<void(Player *)>> PlayerUpdateOnceCallbackList;
static void PlayerUpdate_Proxy(Player *m_pGameObject) {
  // 调用原始函数
  Player_Update_hook.call(m_pGameObject);

  // 执行所有注册的回调
  playerUpdateCallbacks(m_pGameObject);

  // 执行一次性回调并移除它们
  for (auto it = PlayerUpdateOnceCallbackList.begin();
       it != PlayerUpdateOnceCallbackList.end();) {
    (*it)(m_pGameObject);
    it = PlayerUpdateOnceCallbackList.erase(it);
  }
}

void setupU3DPlayerUpdate() {
  void *Player_UpdateAddr =
      IL2CPP::Class::Utils::GetMethodPointer("Player", "Update", 0);
  if (Player_UpdateAddr == nullptr) {
    LogInstance::getMainLogger().error("Failed to find Player::Update");
    return;
  }
  Player_Update_hook =
      safetyhook::create_inline(Player_UpdateAddr, PlayerUpdate_Proxy);
  LogInstance::getMainLogger().debug("PlayerUpdate hooked ");
}
} // namespace zr