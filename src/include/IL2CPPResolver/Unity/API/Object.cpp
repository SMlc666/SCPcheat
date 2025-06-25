#include "Object.hpp"
#include "IL2CPPResolver/API/Class.hpp"
#include "IL2CPPResolver/API/ResolveCall.hpp"
#include "IL2CPPResolver/IL2CPP_Resolver.hpp"
#include "IL2CPPResolver/SystemTypeCache.hpp"
#include "IL2CPPResolver/Unity/Defines.hpp"

Unity::ObjectFunctions_t Unity::m_ObjectFunctions;
void Unity::CObject::Destroy(float fTimeDelay) {
  reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void *, float)>(
      m_ObjectFunctions.m_Destroy)(this, fTimeDelay);
}
Unity::System_String *Unity::CObject::GetName() {
  return reinterpret_cast<System_String *(UNITY_CALLING_CONVENTION)(void *)>(
      m_ObjectFunctions.m_GetName)(this);
}
void Unity::Object::Initialize() {
  IL2CPP::SystemTypeCache::Initializer::Add(UNITY_OBJECT_CLASS);

  m_ObjectFunctions.m_Destroy = IL2CPP::ResolveCall(UNITY_OBJECT_DESTROY);
  m_ObjectFunctions.m_FindObjectsOfType =
      IL2CPP::ResolveCall(UNITY_OBJECT_FINDOBJECTSOFTYPE);
  m_ObjectFunctions.m_GetName = IL2CPP::ResolveCall(UNITY_OBJECT_GETNAME);
  m_ObjectFunctions.m_GetInstanceID = IL2CPP::Class::Utils::GetMethodPointer(
      "UnityEngine.Object", "GetInstanceID", 0);
}
int32_t Unity::CObject::GetInstanceID() {
  return reinterpret_cast<int32_t(UNITY_CALLING_CONVENTION)(void *)>(
      m_ObjectFunctions.m_GetInstanceID)(this);
}
static Unity::il2cppObject *Unity::Object::New(il2cppClass *m_pClass) {
  return reinterpret_cast<Unity::il2cppObject *(
      UNITY_CALLING_CONVENTION)(void *)>(IL2CPP::Functions.m_pObjectNew)(
      m_pClass);
}
