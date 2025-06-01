#include "u3d.hpp"
#include "IL2CPPResolver/IL2CPP_Resolver.hpp"
#include "log/log.hpp"
#include "u3d/sdk/sdk.hpp"
void zr::setupU3D() {
  if (IL2CPP::Initialize(true)) {
    zr::LogInstance::getMainLogger().debug("Il2Cpp initialization successful");
  } else {
    zr::LogInstance::getMainLogger().debug("Il2Cppinitialization failed");
    ExitProcess(0);
  }
  zr::setupU3DSDK();
}