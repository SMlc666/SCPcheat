#pragma once

#include "IL2CPPResolver/API/Class.hpp"
#include "IL2CPPResolver/Unity/Structures/Quaternion.hpp"


namespace Unity {
struct TransformFunctions_t {
  void *m_GetParent = nullptr;
  void *m_GetRoot = nullptr;
  void *m_GetChild = nullptr;
  void *m_GetChildCount = nullptr;
  void *m_FindChild = nullptr;
  void *m_GetPosition = nullptr;
  void *m_GetRotation = nullptr;
  void *m_GetLocalPosition = nullptr;
  void *m_GetLocalScale = nullptr;
  void *m_SetPosition = nullptr;
  void *m_SetRotation = nullptr;
  void *m_SetLocalPosition = nullptr;
  void *m_SetLocalScale = nullptr;
};
extern TransformFunctions_t m_TransformFunctions;

class CTransform : public IL2CPP::CClass {
public:
  CTransform *GetParent();

  CTransform *GetRoot();

  CTransform *GetChild(int m_iIndex);

  int GetChildCount();

  CTransform *FindChild(const char *path, bool isActiveOnly);

  // e.g CGameObject->GetTransform()->FindChild("child1/child2/child3");
  CTransform *FindChild(const char *path);

  Vector3 GetPosition();

  Quaternion GetRotation();

  Vector3 GetLocalPosition();

  Vector3 GetLocalScale();

  void SetPosition(Vector3 m_vVector);

  void SetRotation(Quaternion m_qQuat);

  void SetLocalPosition(Vector3 m_vVector);

  void SetLocalScale(Vector3 m_vVector);
};

namespace Transform {
void Initialize();
} // namespace Transform
} // namespace Unity