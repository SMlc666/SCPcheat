
#include "MainThread.hpp"
#include "config/config.hpp"
#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
                      LPVOID lpReserved) {
  switch (ul_reason_for_call) {
  case DLL_PROCESS_ATTACH:
    zr::createMainThread();
    break;

  case DLL_THREAD_ATTACH:

    break;

  case DLL_THREAD_DETACH:

    break;

  case DLL_PROCESS_DETACH:
    auto configFilePath = zr::ConfigHelper::getConfigFilePath();
    zr::ConfigHelper::saveConfigToFile(configFilePath);
    break;
  }
  return TRUE;
}