#include "GameObject.hpp"
#include "IL2CPPResolver/API/Class.hpp"
#include "IL2CPPResolver/API/ResolveCall.hpp"
#include "IL2CPPResolver/API/String.hpp"
#include "IL2CPPResolver/IL2CPP_Resolver.hpp"
#include "IL2CPPResolver/SystemTypeCache.hpp"
#include "IL2CPPResolver/Unity/Defines.hpp"

Unity::GameObjectFunctions_t Unity::m_GameObjectFunctions;
void Unity::CGameObject::AddComponent(il2cppObject *m_pSystemType) {
  reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void *, void *)>(
      m_GameObjectFunctions.m_AddComponent)(this, m_pSystemType);
}
Unity::CComponent *Unity::CGameObject::GetComponent(const char *m_pName) {
  return reinterpret_cast<CComponent *(
      UNITY_CALLING_CONVENTION)(void *, System_String *)>(
      m_GameObjectFunctions.m_GetComponent)(this, IL2CPP::String::New(m_pName));
}
Unity::CComponent *
Unity::CGameObject::GetComponentInChildren(il2cppObject *m_pSystemType,
                                           bool includeInactive) {
  return reinterpret_cast<CComponent *(UNITY_CALLING_CONVENTION)(void *, void *,
                                                                 bool)>(
      m_GameObjectFunctions.m_GetComponentInChildren)(this, m_pSystemType,
                                                      includeInactive);
}
Unity::CComponent *
Unity::CGameObject::GetComponentInChildren(const char *type) {
  il2cppClass *m_pClass = IL2CPP::Class::Find(type);
  if (!m_pClass)
    return nullptr;

  return GetComponentInChildren(IL2CPP::Class::GetSystemType(m_pClass), true);
}
Unity::il2cppArray<Unity::CComponent *> *
Unity::CGameObject::GetComponents(il2cppObject *m_pSystemType) {
  /*
    0 - Object
    1 - Type
    2 - Use search type as array return type

   * 3 - Recursive
    4 - Include inactive
    5 - Reverse
    6 - Result list

   */
  return reinterpret_cast<Unity::il2cppArray<CComponent *> *(
      UNITY_CALLING_CONVENTION)(void *, void *, bool, bool, bool, bool,
                                void *)>(m_GameObjectFunctions.m_GetComponents)(
      this, m_pSystemType, false, false, true, false, nullptr);
}
Unity::il2cppArray<Unity::CComponent *> *
Unity::CGameObject::GetComponents(const char *m_pSystemTypeName) {
  il2cppClass *m_pClass = IL2CPP::Class::Find(m_pSystemTypeName);
  if (!m_pClass)
    return nullptr;

  return GetComponents(IL2CPP::Class::GetSystemType(m_pClass));
}
Unity::CComponent *
Unity::CGameObject::GetComponentByIndex(il2cppObject *m_pSystemType,
                                        unsigned int m_uIndex) {
  il2cppArray<CComponent *> *m_pComponents = GetComponents(m_pSystemType);
  if (!m_pComponents || m_uIndex >= m_pComponents->m_uMaxLength)
    return nullptr;

  return m_pComponents->operator[](m_uIndex);
}
Unity::CComponent *
Unity::CGameObject::GetComponentByIndex(const char *m_pSystemTypeName,
                                        unsigned int m_uIndex) {
  il2cppClass *m_pClass = IL2CPP::Class::Find(m_pSystemTypeName);
  if (!m_pClass)
    return nullptr;

  return GetComponentByIndex(IL2CPP::Class::GetSystemType(m_pClass), m_uIndex);
}
Unity::CTransform *Unity::CGameObject::GetTransform() {
  return reinterpret_cast<CTransform *(UNITY_CALLING_CONVENTION)(void *)>(
      m_GameObjectFunctions.m_GetTransform)(this);
}
bool Unity::CGameObject::GetActive() {
  return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)(void *)>(
      m_GameObjectFunctions.m_GetActive)(this);
}
unsigned int Unity::CGameObject::GetLayer() {
  return reinterpret_cast<unsigned int(UNITY_CALLING_CONVENTION)(void *)>(
      m_GameObjectFunctions.m_GetLayer)(this);
}
void Unity::GameObject::Initialize() {
  IL2CPP::SystemTypeCache::Initializer::Add(UNITY_GAMEOBJECT_CLASS);
  m_GameObjectFunctions.m_AddComponent =
      IL2CPP::ResolveCall(UNITY_GAMEOBJECT_ADDCOMPONENT);
  m_GameObjectFunctions.m_CreatePrimitive =
      IL2CPP::ResolveCall(UNITY_GAMEOBJECT_CREATEPRIMITIVE);
  m_GameObjectFunctions.m_Find = IL2CPP::ResolveCall(UNITY_GAMEOBJECT_FIND);
  m_GameObjectFunctions.m_FindGameObjectsWithTag =
      IL2CPP::ResolveCall(UNITY_GAMEOBJECT_FINDGAMEOBJECTWITHTAG);
  m_GameObjectFunctions.m_GetComponent =
      IL2CPP::ResolveCall(UNITY_GAMEOBJECT_GETCOMPONENT);
  m_GameObjectFunctions.m_GetComponents =
      IL2CPP::Class::Utils::GetMethodPointer("UnityEngine.GameObject",
                                             "GetComponentsInternal", 6);
  m_GameObjectFunctions.m_GetComponentInChildren =
      IL2CPP::Class::Utils::GetMethodPointer("UnityEngine.GameObject",
                                             "GetComponentInChildren", 2);
  m_GameObjectFunctions.m_GetActive =
      IL2CPP::ResolveCall(UNITY_GAMEOBJECT_GETACTIVE);
  m_GameObjectFunctions.m_GetLayer =
      IL2CPP::ResolveCall(UNITY_GAMEOBJECT_GETLAYER);
  m_GameObjectFunctions.m_GetTransform = IL2CPP::Class::Utils::GetMethodPointer(
      "UnityEngine.GameObject", "get_transform", 0);
  m_GameObjectFunctions.m_SetActive =
      IL2CPP::ResolveCall(UNITY_GAMEOBJECT_SETACTIVE);
  m_GameObjectFunctions.m_SetLayer =
      IL2CPP::ResolveCall(UNITY_GAMEOBJECT_SETLAYER);
}
