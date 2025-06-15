#include "Renderer.hpp"

namespace zr {
bool Renderer::isVisible() { return CallMethod<bool>("get_isVisible"); }
bool Renderer::isEnabled() { return CallMethod<bool>("get_enabled"); }
void Renderer::setEnabled(bool enabled) {
  CallMethod<void>("set_enabled", enabled);
}
Material *Renderer::getMaterial() {
  return CallMethod<Material *>("GetMaterial");
}
void Renderer::setMaterial(Material *material) {
  CallMethod<void>("SetMaterial", material);
}
} // namespace zr