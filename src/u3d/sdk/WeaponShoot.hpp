#pragma once
#include "u3d/sdk/Base/Item/Weapon/Weapon.hpp"
#include <eventpp/callbacklist.h>

namespace zr {
void setupU3DWeaponShoot();
using WeaponDoShootCallbackList = eventpp::CallbackList<void(Weapon *)>;

extern WeaponDoShootCallbackList weaponDoShootCallbacks;
extern std::vector<std::function<void(Weapon *)>> WeaponDoShootOnceCallbackList;

} // namespace zr