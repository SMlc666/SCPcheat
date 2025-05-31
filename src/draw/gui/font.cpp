#include "font.hpp"
#include "imgui.h"

#ifdef _WIN32
#include <Windows.h>
#endif
const static float gFontSizePixels = 15;
void zr::setupImGuiFont() {
#if defined(_WIN32)
  // 加载微软雅黑系统字体
  ImFont *font = ImGui::GetIO().Fonts->AddFontFromFileTTF(
      "C:\\Windows\\Fonts\\msyh.ttc", // 微软雅黑字体路径
      gFontSizePixels,                // 字体大小
      nullptr, ImGui::GetIO().Fonts->GetGlyphRangesChineseFull());

  if (font == nullptr) {
    // 加载失败时使用默认字体
    ImGui::GetIO().Fonts->AddFontDefault();
  }
#elif defined(ANDROID) || defined(__ANDROID__)
  // Android实现保持为空
#else
#error "Unsupported platform"
#endif
}