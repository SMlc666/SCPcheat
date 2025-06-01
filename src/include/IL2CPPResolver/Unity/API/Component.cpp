#include "Component.hpp"

#include "IL2CPPResolver/API/ResolveCall.hpp"
#include "IL2CPPResolver/IL2CPP_Resolver.hpp"
#include "IL2CPPResolver/SystemTypeCache.hpp"
#include "IL2CPPResolver/Unity/Defines.hpp"

Unity::ComponentFunctions_t Unity::m_ComponentFunctions;
Unity::CGameObject *Unity::CComponent::GetGameObject() {
  return reinterpret_cast<CGameObject *(UNITY_CALLING_CONVENTION)(void *)>(
      m_ComponentFunctions.m_GetGameObject)(this);
}
Unity::CTransform *Unity::CComponent::GetTransform() {
  return reinterpret_cast<CTransform *(UNITY_CALLING_CONVENTION)(void *)>(
      m_ComponentFunctions.m_GetTransform)(this);
}
void Unity::Component::Initialize() {
  IL2CPP::SystemTypeCache::Initializer::Add(UNITY_COMPONENT_CLASS);

  m_ComponentFunctions.m_GetGameObject =
      IL2CPP::ResolveCall(UNITY_COMPONENT_GETGAMEOBJECT);
  m_ComponentFunctions.m_GetTransform =
      IL2CPP::ResolveCall(UNITY_COMPONENT_GETTRANSFORM);
}
