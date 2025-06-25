#pragma once

#include "IL2CPPResolver/API/Class.hpp"
#include "IL2CPPResolver/Unity/Structures/Vector3.hpp"
#include "IL2CPPResolver/Unity/Structures/il2cpp.hpp"
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
  if (!m_Collider) {
    return nullptr;
  }
  static void *get_colliderPtr = nullptr;
  if (!get_colliderPtr) {
    get_colliderPtr = IL2CPP::Class::Utils::GetMethodPointer(
        "UnityEngine.RaycastHit", "get_collider", 0);
  }
  if (!get_colliderPtr) {
    return nullptr;
  }
  return reinterpret_cast<void *(UNITY_CALLING_CONVENTION)(void *)>(
      get_colliderPtr)((void *)this);
}
} // namespace Unity