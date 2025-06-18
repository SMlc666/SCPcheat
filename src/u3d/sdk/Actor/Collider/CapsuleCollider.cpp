#pragma once

#include "u3d/sdk/Actor/Collider/CapsuleCollider.hpp"
#include "IL2CPPResolver/Unity/Structures/Vector3.hpp"
namespace zr {
Unity::Vector3 CapsuleCollider::getCenter() {
  return GetPropertyValue<Unity::Vector3>("center");
}
int CapsuleCollider::getDirection() {
  return GetPropertyValue<int>("direction");
}
float CapsuleCollider::getHeight() { return GetPropertyValue<float>("height"); }
float CapsuleCollider::getRadius() { return GetPropertyValue<float>("radius"); }

bool CapsuleCollider::contains(Unity::Vector3 worldPoint) {
  auto transform = GetTransform();
  if (!transform) {
    return false;
  }

  Unity::Vector3 center = getCenter();
  float height = getHeight();
  float radius = getRadius();
  int direction = getDirection();
  Unity::Quaternion rotation = transform->GetRotation();
  Unity::Vector3 position = transform->GetPosition();

  // Transform center to world space
  Unity::Vector3 worldCenter = position + rotation * center;

  // Determine axis direction in world space
  Unity::Vector3 axis;
  switch (direction) {
  case 0: // X-Axis
    axis = rotation * Unity::Vector3{1, 0, 0};
    break;
  case 1: // Y-Axis
    axis = rotation * Unity::Vector3{0, 1, 0};
    break;
  case 2: // Z-Axis
    axis = rotation * Unity::Vector3{0, 0, 1};
    break;
  default:
    return false;
  }

  float halfHeight = std::max(0.0f, height * 0.5f - radius);
  Unity::Vector3 p1 = worldCenter - axis * halfHeight;
  Unity::Vector3 p2 = worldCenter + axis * halfHeight;

  Unity::Vector3 toPoint = worldPoint - p1;
  Unity::Vector3 segment = p2 - p1;
  float t = Unity::Vector3::Dot(toPoint, segment) / segment.sqrMagnitude();
  t = std::max(0.0f, std::min(1.0f, t));

  Unity::Vector3 closestPointOnSegment = p1 + segment * t;
  float distanceSquared = (worldPoint - closestPointOnSegment).sqrMagnitude();

  return distanceSquared <= (radius * radius);
}
} // namespace zr