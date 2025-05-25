#include "GuiInput.hpp"
#include "imgui.h"
#include <windows.h>
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd,
                                                             UINT msg,
                                                             WPARAM wParam,
                                                             LPARAM lParam);
static WNDPROC oWndProc = nullptr;
LRESULT CALLBACK hkWindowProc(_In_ HWND hWnd, _In_ UINT uMsg,
                              _In_ WPARAM wParam, _In_ LPARAM lParam) {
  if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
    return true;
  }
  return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}
void zr::setupImguiInput(HWND hwnd) {
  oWndProc =
      (WNDPROC)::SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)hkWindowProc);
}