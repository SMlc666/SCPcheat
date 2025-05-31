#include "gui.hpp"
#include "draw/gui/font.hpp"
#include "input/input.hpp"
#include "kiero/kiero.h"
#include "log/log.hpp"
#include "safetyhook/easy.hpp"
#include "safetyhook/inline_hook.hpp"
#include "window/WindowManager.hpp"
#include <d3d11.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <iostream>
#include <safetyhook.hpp>

safetyhook::InlineHook Present11_hook;
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
    zr::setupImGuiFont();
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
  zr::LogInstance::getMainLogger().info("Present address:{:p}",
                                        (void *)kiero::getMethodsTable()[8]);
  zr::LogInstance::getMainLogger().flush();
  Present11_hook =
      safetyhook::create_inline(kiero::getMethodsTable()[8], hkPresent11);
  zr::LogInstance::getMainLogger().info("Present hooked");
}