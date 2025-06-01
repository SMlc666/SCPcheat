#include "Transform.hpp"
#include "IL2CPPResolver/API/ResolveCall.hpp"
#include "IL2CPPResolver/API/String.hpp"
#include "IL2CPPResolver/IL2CPP_Resolver.hpp"
#include "IL2CPPResolver/SystemTypeCache.hpp"
#include "IL2CPPResolver/Unity/Defines.hpp"

Unity::TransformFunctions_t Unity::m_TransformFunctions;
Unity::CTransform *Unity::CTransform::GetParent() {
  return reinterpret_cast<CTransform *(UNITY_CALLING_CONVENTION)(void *)>(
      m_TransformFunctions.m_GetParent)(this);
}
Unity::CTransform *Unity::CTransform::GetRoot() {
  return reinterpret_cast<CTransform *(UNITY_CALLING_CONVENTION)(void *)>(
      m_TransformFunctions.m_GetRoot)(this);
}
Unity::CTransform *Unity::CTransform::GetChild(int m_iIndex) {
  return reinterpret_cast<CTransform *(UNITY_CALLING_CONVENTION)(void *, int)>(
      m_TransformFunctions.m_GetChild)(this, m_iIndex);
}
int Unity::CTransform::GetChildCount() {
  return reinterpret_cast<int(UNITY_CALLING_CONVENTION)(void *)>(
      m_TransformFunctions.m_GetChildCount)(this);
}
Unity::CTransform *Unity::CTransform::FindChild(const char *path,
                                                bool isActiveOnly) {
  return reinterpret_cast<CTransform *(
      UNITY_CALLING_CONVENTION)(void *, System_String *, bool)>(
      m_TransformFunctions.m_FindChild)(this, IL2CPP::String::New(path),
                                        isActiveOnly);
}
Unity::CTransform *Unity::CTransform::FindChild(const char *path) {
  if (path == NULL) {
    return nullptr;
  }
  return FindChild(path, false);
}
Unity::Vector3 Unity::CTransform::GetPosition() {
  Vector3 vRet;
  reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void *, Vector3 &)>(
      m_TransformFunctions.m_GetPosition)(this, vRet);
  return vRet;
}
Unity::Quaternion Unity::CTransform::GetRotation() {
  Quaternion m_qRet;
  reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void *, Quaternion &)>(
      m_TransformFunctions.m_GetRotation)(this, m_qRet);
  return m_qRet;
}
void Unity::Transform::Initialize() {
  IL2CPP::SystemTypeCache::Initializer::Add(UNITY_TRANSFORM_CLASS);

  m_TransformFunctions.m_GetParent =
      IL2CPP::ResolveCall(UNITY_TRANSFORM_GETPARENT);
  m_TransformFunctions.m_GetRoot = IL2CPP::ResolveCall(UNITY_TRANSFORM_GETROOT);
  m_TransformFunctions.m_GetChild =
      IL2CPP::ResolveCall(UNITY_TRANSFORM_GETCHILD);
  m_TransformFunctions.m_GetChildCount =
      IL2CPP::ResolveCall(UNITY_TRANSFORM_GETCHILDCOUNT);
  m_TransformFunctions.m_FindChild =
      IL2CPP::ResolveCall(UNITY_TRANSFORM_FINDCHILD);
  m_TransformFunctions.m_GetPosition =
      IL2CPP::ResolveCall(UNITY_TRANSFORM_GETPOSITION);
  m_TransformFunctions.m_GetRotation =
      IL2CPP::ResolveCall(UNITY_TRANSFORM_GETROTATION);
  m_TransformFunctions.m_GetLocalPosition =
      IL2CPP::ResolveCall(UNITY_TRANSFORM_GETLOCALPOSITION);
  m_TransformFunctions.m_GetLocalScale =
      IL2CPP::ResolveCall(UNITY_TRANSFORM_GETLOCALSCALE);
  m_TransformFunctions.m_SetPosition =
      IL2CPP::ResolveCall(UNITY_TRANSFORM_SETPOSITION);
  m_TransformFunctions.m_SetRotation =
      IL2CPP::ResolveCall(UNITY_TRANSFORM_SETROTATION);
  m_TransformFunctions.m_SetLocalPosition =
      IL2CPP::ResolveCall(UNITY_TRANSFORM_SETLOCALPOSITION);
  m_TransformFunctions.m_SetLocalScale =
      IL2CPP::ResolveCall(UNITY_TRANSFORM_SETLOCALSCALE);
}
void Unity::CTransform::SetLocalScale(Vector3 m_vVector) {
  reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void *, Vector3)>(
      m_TransformFunctions.m_SetLocalScale)(this, m_vVector);
}
void Unity::CTransform::SetLocalPosition(Vector3 m_vVector) {
  reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void *, Vector3)>(
      m_TransformFunctions.m_SetLocalPosition)(this, m_vVector);
}
void Unity::CTransform::SetPosition(Vector3 m_vVector) {
  reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void *, Vector3)>(
      m_TransformFunctions.m_SetPosition)(this, m_vVector);
}
void Unity::CTransform::SetRotation(Quaternion m_qQuat) {
  reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void *, Quaternion)>(
      m_TransformFunctions.m_SetRotation)(this, m_qQuat);
}
Unity::Vector3 Unity::CTransform::GetLocalScale() {
  Vector3 vRet;
  reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void *, Vector3 &)>(
      m_TransformFunctions.m_GetLocalScale)(this, vRet);
  return vRet;
}
Unity::Vector3 Unity::CTransform::GetLocalPosition() {
  Vector3 vRet;
  reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void *, Vector3 &)>(
      m_TransformFunctions.m_GetLocalPosition)(this, vRet);
  return vRet;
}
