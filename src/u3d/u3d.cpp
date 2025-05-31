#include "u3d.hpp"
#include "IL2CPPResolver/il2cpp_resolver.hpp"
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