#include "IL2CPP_Resolver.hpp"
// Unity API
#include "Unity/API/Camera.hpp"
#include "Unity/API/Component.hpp"
#include "Unity/API/GameObject.hpp"
#include "Unity/API/LayerMask.hpp"
#include "Unity/API/Object.hpp"
#include "Unity/API/Rigidbody.hpp"
#include "Unity/API/Transform.hpp"

#include "Defines.hpp"

// IL2CPP Headers
#include "Data.hpp"

#include "Unity/Obfuscators.hpp"
#include <unordered_map>

// IL2CPP Headers before Unity API
#include "SystemTypeCache.hpp"

IL2CPP::UnityAPI::m_eExportObfuscationType
    IL2CPP::UnityAPI::m_ExportObfuscation =
        IL2CPP::UnityAPI::m_eExportObfuscationType::None;
int IL2CPP::UnityAPI::m_ROTObfuscationValue = -1;
void *IL2CPP::UnityAPI::ResolveExport(const char *m_Name) {
  switch (m_ExportObfuscation) {
  case m_eExportObfuscationType::ROT: {
    if (m_ROTObfuscationValue == -1) // Bruteforce
    {
      for (int i = 1; 26 > i; ++i) {
        void *m_Return =
            GetProcAddress(Globals.m_GameAssembly,
                           &Unity::Obfuscators::ROT_String(m_Name, i)[0]);
        if (m_Return) {
          m_ROTObfuscationValue = i;
          return m_Return;
        }
      }

      return nullptr;
    }

    return GetProcAddress(
        Globals.m_GameAssembly,
        &Unity::Obfuscators::ROT_String(m_Name, m_ROTObfuscationValue)[0]);
  }
  default:
    return GetProcAddress(Globals.m_GameAssembly, m_Name);
  }

  return nullptr;
}
bool IL2CPP::UnityAPI::ResolveExport_Boolean(void **m_Address,
                                             const char *m_Name) {
  *m_Address = ResolveExport(m_Name);
  IL2CPP_ASSERT(*m_Address != nullptr && "Couldn't resolve export!");
  return (*m_Address);
}
bool IL2CPP::UnityAPI::Initialize() {
  bool m_InitExportResolved = false;
  for (int i = 0; m_eExportObfuscationType::MAX > i; ++i) {
    m_ExportObfuscation = static_cast<m_eExportObfuscationType>(i);
    if (ResolveExport(IL2CPP_INIT_EXPORT)) {
      m_InitExportResolved = true;
      break;
    }
  }

  IL2CPP_ASSERT(m_InitExportResolved && "Couldn't resolve il2cpp_init!");
  if (!m_InitExportResolved)
    return false;

  std::unordered_map<std::string, void **> m_ExportMap = {
      {IL2CPP_CLASS_FROM_NAME_EXPORT, &Functions.m_ClassFromName},
      {IL2CPP_CLASS_GET_FIELDS, &Functions.m_ClassGetFields},
      {IL2CPP_CLASS_GET_FIELD_FROM_NAME_EXPORT,
       &Functions.m_ClassGetFieldFromName},
      {IL2CPP_CLASS_GET_METHODS, &Functions.m_ClassGetMethods},
      {IL2CPP_CLASS_GET_METHOD_FROM_NAME_EXPORT,
       &Functions.m_ClassGetMethodFromName},
      {IL2CPP_CLASS_GET_PROPERTY_FROM_NAME_EXPORT,
       &Functions.m_ClassGetPropertyFromName},
      {IL2CPP_CLASS_GET_TYPE_EXPORT, &Functions.m_ClassGetType},
      {IL2CPP_DOMAIN_GET_EXPORT, &Functions.m_DomainGet},
      {IL2CPP_DOMAIN_GET_ASSEMBLIES_EXPORT, &Functions.m_DomainGetAssemblies},
      {IL2CPP_FREE_EXPORT, &Functions.m_Free},
      {IL2CPP_IMAGE_GET_CLASS_EXPORT, &Functions.m_ImageGetClass},
      {IL2CPP_IMAGE_GET_CLASS_COUNT_EXPORT, &Functions.m_ImageGetClassCount},
      {IL2CPP_RESOLVE_FUNC_EXPORT, &Functions.m_ResolveFunction},
      {IL2CPP_STRING_NEW_EXPORT, &Functions.m_StringNew},
      {IL2CPP_THREAD_ATTACH_EXPORT, &Functions.m_ThreadAttach},
      {IL2CPP_THREAD_DETACH_EXPORT, &Functions.m_ThreadDetach},
      {IL2CPP_TYPE_GET_OBJECT_EXPORT, &Functions.m_TypeGetObject},
      {IL2CPP_OBJECT_NEW, &Functions.m_pObjectNew},
      {IL2CPP_METHOD_GET_PARAM_NAME, &Functions.m_MethodGetParamName},
      {IL2CPP_METHOD_GET_PARAM, &Functions.m_MethodGetParam},
      {IL2CPP_CLASS_FROM_IL2CPP_TYPE, &Functions.m_ClassFromIl2cppType},
      {IL2CPP_FIELD_STATIC_GET_VALUE, &Functions.m_FieldStaticGetValue},
      {IL2CPP_FIELD_STATIC_SET_VALUE, &Functions.m_FieldStaticSetValue},
      {IL2CPP_VALUE_BOX, &Functions.m_ValueBox}
  };

  for (auto &m_ExportPair : m_ExportMap) {
    if (!ResolveExport_Boolean(m_ExportPair.second, &m_ExportPair.first[0]))
      return false;
  }

  // Unity APIs
  Unity::Camera::Initialize();
  Unity::Component::Initialize();
  Unity::GameObject::Initialize();
  Unity::LayerMask::Initialize();
  Unity::Object::Initialize();
  Unity::RigidBody::Initialize();
  Unity::Transform::Initialize();

  // Caches
  IL2CPP::SystemTypeCache::Initializer::PreCache();

  return true;
}
bool IL2CPP::Initialize(bool m_WaitForModule, int m_MaxSecondsWait) {
  Globals.m_GameAssembly = GetModuleHandleA(IL2CPP_MAIN_MODULE);

  if (m_WaitForModule) {
    int m_SecondsWaited = 0;
    while (!Globals.m_GameAssembly) {
      if (m_SecondsWaited >= m_MaxSecondsWait)
        return false;

      Globals.m_GameAssembly = GetModuleHandleA(IL2CPP_MAIN_MODULE);
      ++m_SecondsWaited;
      Sleep(1000);
    }
  }

  if (!UnityAPI::Initialize())
    return false;

  return true;
}
