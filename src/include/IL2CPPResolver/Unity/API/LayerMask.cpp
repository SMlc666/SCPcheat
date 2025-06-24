#include "LayerMask.hpp"
#include "IL2CPPResolver/API/Class.hpp"
#include "IL2CPPResolver/API/ResolveCall.hpp"
#include "IL2CPPResolver/API/String.hpp"
#include "IL2CPPResolver/IL2CPP_Resolver.hpp"
#include "IL2CPPResolver/SystemTypeCache.hpp"
#include "IL2CPPResolver/Unity/Defines.hpp"
#include "log/log.hpp"

Unity::LayerMaskFunctions_t Unity::m_LayerMaskFunctions;
void Unity::LayerMask::Initialize() {
  IL2CPP::SystemTypeCache::Initializer::Add(UNITY_LAYERMASK_CLASS);

  m_LayerMaskFunctions.m_LayerToName = IL2CPP::Class::Utils::GetMethodPointer(
      "UnityEngine.LayerMask", "LayerToName", 1);
  if (m_LayerMaskFunctions.m_LayerToName == nullptr) {
    zr::LogInstance::getMainLogger().info("cannot resolve LayerToName");
  }
  m_LayerMaskFunctions.m_NameToLayer = IL2CPP::Class::Utils::GetMethodPointer(
      "UnityEngine.LayerMask", "NameToLayer", 1);
  if (m_LayerMaskFunctions.m_NameToLayer == nullptr) {
    zr::LogInstance::getMainLogger().info("cannot resolve NameToLayer");
  }
}
Unity::System_String *Unity::LayerMask::LayerToName(unsigned int m_uLayer) {
  return reinterpret_cast<System_String *(
      UNITY_CALLING_CONVENTION)(unsigned int)>(
      m_LayerMaskFunctions.m_LayerToName)(m_uLayer);
}
uint32_t Unity::LayerMask::NameToLayer(const char *m_pName) {
  return reinterpret_cast<uint32_t(UNITY_CALLING_CONVENTION)(void *)>(
      m_LayerMaskFunctions.m_NameToLayer)(IL2CPP::String::New(m_pName));
}
