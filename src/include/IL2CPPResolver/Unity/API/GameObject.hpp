#pragma once

#include "IL2CPPResolver/Unity/API/Component.hpp"
#include "IL2CPPResolver/Unity/API/Object.hpp"
#include "IL2CPPResolver/Unity/Structures/il2cpp.hpp"

namespace Unity {
struct GameObjectFunctions_t {
  void *m_AddComponent = nullptr;
  void *m_CreatePrimitive = nullptr;
  void *m_Find = nullptr;
  void *m_FindGameObjectsWithTag = nullptr;
  void *m_GetComponent = nullptr;
  void *m_GetComponents = nullptr;
  void *m_GetComponentInChildren = nullptr;
  void *m_GetActive = nullptr;
  void *m_GetLayer = nullptr;
  void *m_GetTransform = nullptr;
  void *m_SetActive = nullptr;
  void *m_SetLayer = nullptr;
};
extern GameObjectFunctions_t m_GameObjectFunctions;

class CGameObject : public CObject {
public:
  void AddComponent(il2cppObject *m_pSystemType);

  CComponent *GetComponent(const char *m_pName);

  CComponent *GetComponentInChildren(il2cppObject *m_pSystemType,
                                     bool includeInactive);

  // e.g CGameObject->GetComponentInChildren("Namespace.Component");
  CComponent *GetComponentInChildren(const char *type);

  il2cppArray<CComponent *> *GetComponents(il2cppObject *m_pSystemType);

  il2cppArray<CComponent *> *GetComponents(const char *m_pSystemTypeName);

  CComponent *GetComponentByIndex(il2cppObject *m_pSystemType,
                                  unsigned int m_uIndex = 0U);

  CComponent *GetComponentByIndex(const char *m_pSystemTypeName,
                                  unsigned int m_uIndex = 0U);

  CTransform *GetTransform();

  bool GetActive();

  unsigned int GetLayer();

  /*
   * (WARNING) when you use GameObject::Find and you set the object to unactive,
   * you won't find it anymore with that slow function.
   */
  void SetActive(bool m_bActive);

  void SetLayer(unsigned int m_uLayer);
};

namespace GameObject {
enum class m_ePrimitiveType : int {
  Default = 0,
  Sphere = 0,
  Capsule,
  Cylinder,
  Cube,
  Plane,
  Quad,
};

void Initialize();

CGameObject *CreatePrimitive(m_ePrimitiveType m_Type);

CGameObject *Find(const char *m_Name);

il2cppArray<CGameObject *> *FindWithTag(const char *m_Tag);
} // namespace GameObject
} // namespace Unity
