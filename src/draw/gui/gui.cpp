#include "gui.hpp"
#include "hook/hook.hpp"
#include "input/input.hpp"
#include "kiero/kiero.h"
#include "window/WindowManager.hpp"
#include <d3d11.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

zr::Hook Present11_hook;
long __stdcall hkPresent11(IDXGISwapChain *pSwapChain, UINT SyncInterval,
                           UINT Flags) {
  static bool init = false;

  if (!init) {
    DXGI_SWAP_CHAIN_DESC desc;
    pSwapChain->GetDesc(&desc);

    ID3D11Device *device;
    pSwapChain->GetDevice(__uuidof(ID3D11Device), (void **)&device);

    ID3D11DeviceContext *context;
    device->GetImmediateContext(&context);

    zr::setupInput((HWND)desc.OutputWindow);
    ImGui::CreateContext();
    ImGui_ImplWin32_Init(desc.OutputWindow);
    ImGui_ImplDX11_Init(device, context);

    init = true;
  }

  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();
  zr::WindowManager::RenderAll();
  ImGui::EndFrame();
  ImGui::Render();
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

  return Present11_hook.stdcall<long>(pSwapChain, SyncInterval, Flags);
}

void zr::setupImGui() {
  Present11_hook = zr::Hook((void *)kiero::getMethodsTable()[8], hkPresent11);
}