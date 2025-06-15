#pragma once
#include "IL2CPPResolver/Unity/API/Component.hpp"
#include "u3d/sdk/Actor/Renderer/Material.hpp"
namespace zr {
class Renderer : public Unity::CComponent {
public:
  bool isVisible();
  bool isEnabled();
  void setEnabled(bool enabled);
  Material *getMaterial();
  void setMaterial(Material *material);
};
} // namespace zr