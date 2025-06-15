#include "WeaponShoot.hpp"
#include "IL2CPPResolver/Unity/Structures/Engine.hpp"
#include "log/log.hpp"
#include "safetyhook.hpp"
#include <cstdint>

safetyhook::InlineHook Weapon_DoShoot_hook;
namespace zr {

WeaponDoShootCallbackList weaponDoShootCallbacks;
std::vector<std::function<void(Weapon *)>> WeaponDoShootOnceCallbackList;
static void Weapon_DoShoot_Proxy(Weapon *m_pWeapon) {
  // 调用原始函数
  Weapon_DoShoot_hook.call(m_pWeapon);
  // 执行所有注册的回调
  weaponDoShootCallbacks(m_pWeapon);

  // 执行一次性回调并移除它们
  for (auto it = WeaponDoShootOnceCallbackList.begin();
       it != WeaponDoShootOnceCallbackList.end();) {
    (*it)(m_pWeapon);
    it = WeaponDoShootOnceCallbackList.erase(it);
  }
}
void setupU3DWeaponShoot() {
  void *Weapon_DoShootAddr =
      IL2CPP::Class::Utils::GetMethodPointer("Akequ.Base.Weapon", "DoShoot", 0);
  if (Weapon_DoShootAddr == nullptr) {
    LogInstance::getMainLogger().error("Failed to find Weapon::DoShoot");
    return;
  }
  Weapon_DoShoot_hook =
      safetyhook::create_inline(Weapon_DoShootAddr, Weapon_DoShoot_Proxy);
}
} // namespace zr
