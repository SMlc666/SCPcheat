#include "Transform.hpp"
#include "IL2CPPResolver/API/Class.hpp"
#include "IL2CPPResolver/API/ResolveCall.hpp"
#include "IL2CPPResolver/API/String.hpp"
#include "IL2CPPResolver/IL2CPP_Resolver.hpp"
#include "IL2CPPResolver/SystemTypeCache.hpp"
#include "IL2CPPResolver/Unity/Defines.hpp"
#include "IL2CPPResolver/Unity/Structures/Engine.hpp"
#include "log/log.hpp"

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
  vRet = CallMethod<Vector3>("get_position");
  return vRet;
}
Unity::Quaternion Unity::CTransform::GetRotation() {
  Quaternion m_qRet;
  m_qRet = CallMethod<Quaternion>("get_rotation");
  return m_qRet;
}
void Unity::Transform::Initialize() {
  IL2CPP::SystemTypeCache::Initializer::Add(UNITY_TRANSFORM_CLASS);

  m_TransformFunctions.m_GetParent =
      IL2CPP::ResolveCall(UNITY_TRANSFORM_GETPARENT);
  if (!m_TransformFunctions.m_GetParent) {
    zr::LogInstance::getMainLogger().error(
        "Failed to resolve Unity.Transform::get_parent");
  }
  m_TransformFunctions.m_GetRoot = IL2CPP::Class::Utils::GetMethodPointer(
      "UnityEngine.Transform", "GetRoot", 0);
  if (!m_TransformFunctions.m_GetRoot) {
    zr::LogInstance::getMainLogger().error(
        "Failed to resolve Unity.Transform::GetRoot");
  }

  m_TransformFunctions.m_GetChild =
      IL2CPP::ResolveCall(UNITY_TRANSFORM_GETCHILD);
  if (!m_TransformFunctions.m_GetChild) {
    zr::LogInstance::getMainLogger().error(
        "Failed to resolve Unity.Transform::GetChild");
  }
  m_TransformFunctions.m_GetChildCount =
      IL2CPP::ResolveCall(UNITY_TRANSFORM_GETCHILDCOUNT);
  if (!m_TransformFunctions.m_GetChildCount) {
    zr::LogInstance::getMainLogger().error(
        "Failed to resolve Unity.Transform::get_childCount");
  }
  m_TransformFunctions.m_FindChild =
      IL2CPP::ResolveCall(UNITY_TRANSFORM_FINDCHILD);
  if (!m_TransformFunctions.m_FindChild) {
    zr::LogInstance::getMainLogger().error(
        "Failed to resolve Unity.Transform::FindRelativeTransformWithPath");
  }
  m_TransformFunctions.m_GetPosition =
      IL2CPP::ResolveCall(UNITY_TRANSFORM_GETPOSITION);
  if (!m_TransformFunctions.m_GetPosition) {
    zr::LogInstance::getMainLogger().error(
        "Failed to resolve Unity.Transform::get_position_Injected");
  }
  m_TransformFunctions.m_GetRotation =
      IL2CPP::ResolveCall(UNITY_TRANSFORM_GETROTATION);
  if (!m_TransformFunctions.m_GetRotation) {
    zr::LogInstance::getMainLogger().error(
        "Failed to resolve Unity.Transform::get_rotation_Injected");
  }

  m_TransformFunctions.m_GetLocalPosition =
      IL2CPP::ResolveCall(UNITY_TRANSFORM_GETLOCALPOSITION);
  if (!m_TransformFunctions.m_GetLocalPosition) {
    zr::LogInstance::getMainLogger().error(
        "Failed to resolve Unity.Transform::get_localPosition_Injected");
  }
  m_TransformFunctions.m_GetLocalScale =
      IL2CPP::ResolveCall(UNITY_TRANSFORM_GETLOCALSCALE);
  if (!m_TransformFunctions.m_GetLocalScale) {
    zr::LogInstance::getMainLogger().error(
        "Failed to resolve Unity.Transform::get_localScale_Injected");
  }
  m_TransformFunctions.m_SetPosition =
      IL2CPP::ResolveCall(UNITY_TRANSFORM_SETPOSITION);
  if (!m_TransformFunctions.m_SetPosition) {
    zr::LogInstance::getMainLogger().error(
        "Failed to resolve Unity.Transform::get_position_Injected");
  }

  m_TransformFunctions.m_SetRotation =
      IL2CPP::ResolveCall(UNITY_TRANSFORM_SETROTATION);
  if (!m_TransformFunctions.m_SetRotation) {
    zr::LogInstance::getMainLogger().error(
        "Failed to resolve Unity.Transform::get_rotation_Injected");
  }
  m_TransformFunctions.m_SetLocalPosition =
      IL2CPP::ResolveCall(UNITY_TRANSFORM_SETLOCALPOSITION);
  if (!m_TransformFunctions.m_SetLocalPosition) {
    zr::LogInstance::getMainLogger().error(
        "Failed to resolve Unity.Transform::get_localPosition_Injected");
  }
  m_TransformFunctions.m_SetLocalScale =
      IL2CPP::ResolveCall(UNITY_TRANSFORM_SETLOCALSCALE);
  m_TransformFunctions.m_GetForward = IL2CPP::Class::Utils::GetMethodPointer(
      "UnityEngine.Transform", "get_forward", 0);
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

Unity::Vector3 Unity::CTransform::GetForward() {
  return reinterpret_cast<Vector3(UNITY_CALLING_CONVENTION)(void *)>(
      m_TransformFunctions.m_GetForward)(this);
}
