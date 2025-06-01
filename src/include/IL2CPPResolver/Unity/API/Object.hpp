#pragma once
#include "IL2CPPResolver/API/Class.hpp"
#include "IL2CPPResolver/Unity/Structures/System_String.hpp"
#include "IL2CPPResolver/Unity/Structures/il2cppArray.hpp"
namespace Unity {
struct ObjectFunctions_t {
  void *m_Destroy = nullptr;
  void *m_FindObjectsOfType = nullptr;
  void *m_GetName = nullptr;
};
extern ObjectFunctions_t m_ObjectFunctions;

class CObject : public IL2CPP::CClass {
public:
  void Destroy(float fTimeDelay = 0.f);

  System_String *GetName();
};

namespace Object {
void Initialize();

static il2cppObject *New(il2cppClass *m_pClass);

template <typename T>
static il2cppArray<T *> *FindObjectsOfType(il2cppObject *m_pSystemType,
                                           bool m_bIncludeInactive = false) {
  return reinterpret_cast<Unity::il2cppArray<T *> *(
      UNITY_CALLING_CONVENTION)(void *, bool)>(
      m_ObjectFunctions.m_FindObjectsOfType)(m_pSystemType, m_bIncludeInactive);
}

template <typename T>
static il2cppArray<T *> *FindObjectsOfType(const char *m_pSystemTypeName,
                                           bool m_bIncludeInactive = false) {
  il2cppClass *m_pClass = IL2CPP::Class::Find(m_pSystemTypeName);
  if (!m_pClass)
    return nullptr;

  return FindObjectsOfType<T>(IL2CPP::Class::GetSystemType(m_pClass),
                              m_bIncludeInactive);
}

template <typename T>
static T *FindObjectOfType(il2cppObject *m_pSystemType,
                           bool m_bIncludeInactive = false) {
  il2cppArray<T *> *m_pArray =
      FindObjectsOfType<T>(m_pSystemType, m_bIncludeInactive);
  if (!m_pArray || m_pArray->m_uMaxLength == 0U)
    return nullptr;

  return m_pArray->m_pValues[0];
}

template <typename T>
static T *FindObjectOfType(const char *m_pSystemTypeName,
                           bool m_bIncludeInactive = false) {
  il2cppClass *m_pClass = IL2CPP::Class::Find(m_pSystemTypeName);
  if (!m_pClass)
    return nullptr;

  return FindObjectOfType<T>(IL2CPP::Class::GetSystemType(m_pClass),
                             m_bIncludeInactive);
}
} // namespace Object
} // namespace Unity
