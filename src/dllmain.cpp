#include "draw/draw.hpp"
#include <thread>
#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
                      LPVOID lpReserved) {
  switch (ul_reason_for_call) {
  case DLL_PROCESS_ATTACH:
    std::thread([]() {
#ifdef LOG_TO_CONSOLE
      if (GetConsoleWindow() == NULL) {
        AllocConsole();
        // 安全地重定向标准输出到新控制台
        FILE *new_stdout;
        freopen_s(&new_stdout, "CONOUT$", "w", stdout);
        FILE *new_stderr;
        freopen_s(&new_stderr, "CONOUT$", "w", stderr);
        FILE *new_stdin;
        freopen_s(&new_stdin, "CONIN$", "r", stdin);

        SetConsoleTitleA("LOG CONSOLE");
      }
#endif
      zr::setupDraw();
    }).detach();
    break;

  case DLL_THREAD_ATTACH:

    break;

  case DLL_THREAD_DETACH:

    break;

  case DLL_PROCESS_DETACH:

    break;
  }
  return TRUE;
}