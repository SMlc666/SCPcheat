#include "sdk.hpp"
#include "u3d/sdk/ModuleBase.hpp"
#include "u3d/sdk/PlayerUpdate.hpp"
#include "u3d/sdk/WeaponShoot.hpp"
#include <Windows.h>

void zr::setupU3DSDK() {
  setupU3DModuleBase();
  setupU3DPlayerUpdate();
  setupU3DWeaponShoot();
}