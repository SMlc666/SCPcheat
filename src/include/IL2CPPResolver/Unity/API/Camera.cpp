#include "Camera.hpp"
#include "IL2CPPResolver/API/Class.hpp"
#include "IL2CPPResolver/API/ResolveCall.hpp"
#include "IL2CPPResolver/IL2CPP_Resolver.hpp"
#include "IL2CPPResolver/SystemTypeCache.hpp"
#include "IL2CPPResolver/Unity/Defines.hpp"
#include "imgui.h"
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
bool Unity::CCamera::WorldToScreen(Unity::Vector3 world, Vector2 &screen) {

  Unity::Vector3 buffer = CallMethodSafe<Unity::Vector3>(
      "WorldToScreenPoint", world, m_eCameraEye_Center);
  float ScreenSizeX = ImGui::GetIO().DisplaySize.x;
  float ScreenSizeY = ImGui::GetIO().DisplaySize.y;
  if (buffer.x > ScreenSizeX || buffer.y > ScreenSizeY || buffer.x < 0 ||
      buffer.y < 0 || buffer.z < 0) {
    return false;
  }

  if (buffer.z > 0.0f) {
    screen = Vector2(buffer.x, ScreenSizeY - buffer.y);
  }

  if (screen.x > 0 || screen.y > 0) {
    return true;
  }
  return false;
}
void Unity::Camera::Initialize() {
  IL2CPP::SystemTypeCache::Initializer::Add(UNITY_CAMERA_CLASS);

  m_CameraFunctions.m_GetCurrent = IL2CPP::Class::Utils::GetMethodPointer(
      "UnityEngine.Camera", "get_current", 0);
  m_CameraFunctions.m_GetMain = IL2CPP::Class::Utils::GetMethodPointer(
      "UnityEngine.Camera", "get_main", 0);
  m_CameraFunctions.m_GetDepth = IL2CPP::ResolveCall(UNITY_CAMERA_GETDEPTH);
  m_CameraFunctions.m_SetDepth = IL2CPP::ResolveCall(UNITY_CAMERA_SETDEPTH);
  m_CameraFunctions.m_GetFieldOfView =
      IL2CPP::ResolveCall(UNITY_CAMERA_GETFIELDOFVIEW);
  m_CameraFunctions.m_SetFieldOfView =
      IL2CPP::ResolveCall(UNITY_CAMERA_SETFIELDOFVIEW);
  m_CameraFunctions.m_WorldToScreen = IL2CPP::Class::Utils::GetMethodPointer(
      "UnityEngine.Camera", "WorldToScreenPoint_Injected", 4);
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