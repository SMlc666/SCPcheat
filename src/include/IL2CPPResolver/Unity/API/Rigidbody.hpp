#pragma once
#include "IL2CPPResolver/API/Class.hpp"

#include "IL2CPPResolver/Unity/Structures/Engine.hpp"

namespace Unity {
struct RigidbodyFunctions_t {
  void *m_GetDetectCollisions = nullptr;
  void *m_GetVelocity = nullptr;
  void *m_SetDetectCollisions = nullptr;
  void *m_SetVelocity = nullptr;
};
extern RigidbodyFunctions_t m_RigidbodyFunctions;

class CRigidbody : public IL2CPP::CClass {
public:
  bool GetDetectCollisions();

  void SetDetectCollisions(bool m_bDetect);

  Vector3 GetVelocity();

  void SetVelocity(Vector3 m_vVector);
};

namespace RigidBody {
void Initialize();
} // namespace RigidBody
} // namespace Unity