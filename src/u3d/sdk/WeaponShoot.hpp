#pragma once
#include "u3d/sdk/Base/Item/Weapon/Weapon.hpp"
#include <eventpp/callbacklist.h>

namespace zr {
void setupU3DWeaponShoot();
using WeaponShootServerCallbackList = eventpp::CallbackList<void(Weapon *)>;

extern WeaponShootServerCallbackList weaponShootServerCallbacks;
extern std::vector<std::function<void(Weapon *)>>
    WeaponShootServerOnceCallbackList;


} // namespace zr