#include "draw.hpp"
#include "gui/gui.hpp"
#include "include/kiero/kiero.h"
void zr::setupDraw() {
  while (true) {
    if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success) {
      setupImGui();
      break;
    }
  }
}