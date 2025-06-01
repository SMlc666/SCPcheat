#include "Camera.hpp"
#include "IL2CPPResolver/API/ResolveCall.hpp"
#include "IL2CPPResolver/IL2CPP_Resolver.hpp"
#include "IL2CPPResolver/SystemTypeCache.hpp"
#include "IL2CPPResolver/Unity/Defines.hpp"

float Unity::CCamera::GetDepth() {
  return reinterpret_cast<float(UNITY_CALLING_CONVENTION)(void *)>(
      m_CameraFunctions.m_GetDepth)(this);
}
void Unity::CCamera::SetDepth(float m_fValue) {
  reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void *, float)>(
      m_CameraFunctions.m_SetDepth)(this, m_fValue);
}
float Unity::CCamera::GetFieldOfView() {
  return reinterpret_cast<float(UNITY_CALLING_CONVENTION)(void *)>(
      m_CameraFunctions.m_GetFieldOfView)(this);
}
void Unity::CCamera::SetFieldOfView(float m_fValue) {
  reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void *, float)>(
      m_CameraFunctions.m_SetFieldOfView)(this, m_fValue);
}
void Unity::CCamera::WorldToScreen(Vector3 &m_vWorld, Vector3 &m_vScreen,
                                   int m_iEye) {
  reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void *, Vector3 &, int,
                                                  Vector3 &)>(
      m_CameraFunctions.m_WorldToScreen)(this, m_vWorld, m_iEye, m_vScreen);
}
void Unity::Camera::Initialize() {
  IL2CPP::SystemTypeCache::Initializer::Add(UNITY_CAMERA_CLASS);

  m_CameraFunctions.m_GetCurrent = IL2CPP::ResolveCall(UNITY_CAMERA_GETCURRENT);
  m_CameraFunctions.m_GetMain = IL2CPP::ResolveCall(UNITY_CAMERA_GETMAIN);
  m_CameraFunctions.m_GetDepth = IL2CPP::ResolveCall(UNITY_CAMERA_GETDEPTH);
  m_CameraFunctions.m_SetDepth = IL2CPP::ResolveCall(UNITY_CAMERA_SETDEPTH);
  m_CameraFunctions.m_GetFieldOfView =
      IL2CPP::ResolveCall(UNITY_CAMERA_GETFIELDOFVIEW);
  m_CameraFunctions.m_SetFieldOfView =
      IL2CPP::ResolveCall(UNITY_CAMERA_SETFIELDOFVIEW);
  m_CameraFunctions.m_WorldToScreen =
      IL2CPP::ResolveCall(UNITY_CAMERA_WORLDTOSCREEN);
}
Unity::CCamera *Unity::Camera::GetCurrent() {
  return reinterpret_cast<CCamera *(UNITY_CALLING_CONVENTION)()>(
      m_CameraFunctions.m_GetCurrent)();
}
Unity::CCamera *Unity::Camera::GetMain() {
  return reinterpret_cast<CCamera *(UNITY_CALLING_CONVENTION)()>(
      m_CameraFunctions.m_GetMain)();
}

Unity::CameraFunctions_t Unity::m_CameraFunctions;