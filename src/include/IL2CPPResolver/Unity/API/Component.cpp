#include "Component.hpp"

#include "IL2CPPResolver/API/Class.hpp"
#include "IL2CPPResolver/API/ResolveCall.hpp"
#include "IL2CPPResolver/API/String.hpp"
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
Unity::CComponent *
Unity::CComponent::getComponent(const char *m_pComponentTypeName) {
  return reinterpret_cast<CComponent *(
      UNITY_CALLING_CONVENTION)(void *, Unity::System_String *)>(
      m_ComponentFunctions.m_GetComponent)(
      this, IL2CPP::String::New(m_pComponentTypeName));
}
void Unity::Component::Initialize() {
  IL2CPP::SystemTypeCache::Initializer::Add(UNITY_COMPONENT_CLASS);

  m_ComponentFunctions.m_GetGameObject = IL2CPP::Class::Utils::GetMethodPointer(
      "UnityEngine.Component", "get_gameObject", 0);
  m_ComponentFunctions.m_GetTransform = IL2CPP::Class::Utils::GetMethodPointer(
      "UnityEngine.Component", "get_transform", 0);
  m_ComponentFunctions.m_GetComponent = IL2CPP::Class::Utils::GetMethodPointer(
      "UnityEngine.Component", "GetComponent", {"System.String"});
}
