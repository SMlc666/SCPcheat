#include "WeaponShoot.hpp"
#include "IL2CPPResolver/Unity/Structures/Engine.hpp"
#include "log/log.hpp"
#include "safetyhook.hpp"
#include <cstdint>

safetyhook::InlineHook Weapon_ShootServer_hook;
namespace zr {

WeaponShootServerCallbackList weaponShootServerCallbacks;
std::vector<std::function<void(Weapon *)>> WeaponShootServerOnceCallbackList;
static void Weapon_ShootServer_Proxy(Weapon *m_pWeapon, uint16_t playerId,
                                     Unity::Vector3 direction) {
  // 调用原始函数
  Weapon_ShootServer_hook.call(m_pWeapon, playerId, direction);
  // 执行所有注册的回调
  weaponShootServerCallbacks(m_pWeapon);

  // 执行一次性回调并移除它们
  for (auto it = WeaponShootServerOnceCallbackList.begin();
       it != WeaponShootServerOnceCallbackList.end();) {
    (*it)(m_pWeapon);
    it = WeaponShootServerOnceCallbackList.erase(it);
  }
}
void setupU3DWeaponShoot() {
  void *Weapon_ShootServerAddr = IL2CPP::Class::Utils::GetMethodPointer(
      "Akequ.Base.Weapon", "ShootServer", 2);
  if (Weapon_ShootServerAddr == nullptr) {
    LogInstance::getMainLogger().error("Failed to find Weapon::ShootServer");
    return;
  }
  Weapon_ShootServer_hook = safetyhook::create_inline(Weapon_ShootServerAddr,
                                                      Weapon_ShootServer_Proxy);
}
} // namespace zr
