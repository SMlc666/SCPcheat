#include "Rigidbody.hpp"

#include "IL2CPPResolver/API/ResolveCall.hpp"
#include "IL2CPPResolver/IL2CPP_Resolver.hpp"
#include "IL2CPPResolver/SystemTypeCache.hpp"
#include "IL2CPPResolver/Unity/Defines.hpp"

Unity::RigidbodyFunctions_t Unity::m_RigidbodyFunctions;
bool Unity::CRigidbody::GetDetectCollisions() {
  return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)(void *)>(
      m_RigidbodyFunctions.m_GetDetectCollisions)(this);
}
void Unity::CRigidbody::SetDetectCollisions(bool m_bDetect) {
  reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void *, bool)>(
      m_RigidbodyFunctions.m_SetDetectCollisions)(this, m_bDetect);
}
Unity::Vector3 Unity::CRigidbody::GetVelocity() {
  Vector3 vRet;
  reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void *, Vector3 &)>(
      m_RigidbodyFunctions.m_GetVelocity)(this, vRet);
  return vRet;
}
void Unity::CRigidbody::SetVelocity(Vector3 m_vVector) {
  reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void *, Vector3)>(
      m_RigidbodyFunctions.m_SetVelocity)(this, m_vVector);
}
void Unity::RigidBody::Initialize() {
  IL2CPP::SystemTypeCache::Initializer::Add(UNITY_RIGIDBODY_CLASS);

  m_RigidbodyFunctions.m_GetDetectCollisions =
      IL2CPP::ResolveCall(UNITY_RIGIDBODY_GETDETECTCOLLISIONS);
  m_RigidbodyFunctions.m_GetVelocity =
      IL2CPP::ResolveCall(UNITY_RIGIDBODY_GETVELOCITY);
  m_RigidbodyFunctions.m_SetDetectCollisions =
      IL2CPP::ResolveCall(UNITY_RIGIDBODY_SETDETECTCOLLISIONS);
  m_RigidbodyFunctions.m_SetVelocity =
      IL2CPP::ResolveCall(UNITY_RIGIDBODY_SETVELOCITY);
}
