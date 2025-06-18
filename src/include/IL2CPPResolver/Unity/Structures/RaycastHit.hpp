#pragma once

#include "IL2CPPResolver/API/Class.hpp"
#include "IL2CPPResolver/Unity/Structures/Vector3.hpp"
#include <cstdint>
#include <fmt/core.h> // 包含 fmt 库头文件
namespace Unity {

struct RaycastHit {
  Vector3 point{}, normal{};
  unsigned int faceID{};
  float distance{};
  Vector2 UV{};
  int m_Collider{};
  inline void *GetCollider() const;

  inline std::string str() {
    return fmt::format("Point: {}, Normal: {}, FaceID: {}, Distance: {}, UV: "
                       "{}, m_Collider: {:p}",
                       point.str(), normal.str(), faceID, distance, UV.str(),
                       (void *)m_Collider);
  }
};
void *RaycastHit::GetCollider() const {
  if (!m_Collider || (uintptr_t)m_Collider < 0)
    return {};
  static void *(*FromId)(int){};
  if (!FromId) {
    FromId = (decltype(FromId))IL2CPP::Class::Utils::GetMethodPointer(
        "UnityEngine.Object", "FindObjectFromInstanceID", 1);
  }
  auto gcHandle = (long)FromId(m_Collider);
  gcHandle = gcHandle & ~(long)1;
  return (void *)gcHandle;
}
} // namespace Unity