#include "ModuleBase.hpp"
#include "log/log.hpp"
#include <Windows.h>
#include <cstdint>

static uintptr_t Base;
static uintptr_t GameAssemblyBase;
static uintptr_t UnityPlayerBase;

void zr::setupU3DModuleBase() {
  Base = (uintptr_t)GetModuleHandleA(nullptr);
  zr::LogInstance::getMainLogger().info("Base: {:p}",
                                        reinterpret_cast<void *>(Base));
  GameAssemblyBase =
      reinterpret_cast<uintptr_t>(GetModuleHandleA("GameAssembly.dll"));
  zr::LogInstance::getMainLogger().info(
      "GameAssemblyBase: {:p}", reinterpret_cast<void *>(GameAssemblyBase));
  UnityPlayerBase =
      reinterpret_cast<uintptr_t>(GetModuleHandleA("UnityPlayer.dll"));
  zr::LogInstance::getMainLogger().info(
      "UnityPlayerBase: {:p}", reinterpret_cast<void *>(UnityPlayerBase));
}
uintptr_t zr::getProcessBase() { return Base; }
uintptr_t zr::getGameAssemblyBase() { return GameAssemblyBase; }
uintptr_t zr::getUnityPlayerBase() { return UnityPlayerBase; }