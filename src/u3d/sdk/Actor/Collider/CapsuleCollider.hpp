#pragma once

#include "IL2CPPResolver/Unity/Structures/Vector3.hpp"
#include "u3d/sdk/Actor/Collider/Collider.hpp"
namespace zr {
class CapsuleCollider : public Collider {
public:
  Unity::Vector3 getCenter();
  int getDirection();
  float getHeight();
  float getRadius();
  bool contains(Unity::Vector3 worldPoint);
};
} // namespace zr