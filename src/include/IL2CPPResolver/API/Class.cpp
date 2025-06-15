#include "Class.hpp"
#include "IL2CPPResolver/API/Domain.hpp"
#include <cstring>
#include <fmt/core.h>

Unity::il2cppFieldInfo *IL2CPP::Class::GetFields(Unity::il2cppClass *m_pClass,
                                                 void **m_pIterator) {
  return reinterpret_cast<Unity::il2cppFieldInfo *(
      IL2CPP_CALLING_CONVENTION)(void *, void **)>(Functions.m_ClassGetFields)(
      m_pClass, m_pIterator);
}
void IL2CPP::Class::FetchFields(
    Unity::il2cppClass *m_pClass,
    std::vector<Unity::il2cppFieldInfo *> *m_pVector, void *m_pFieldIterator) {
  m_pVector->clear();

  while (1) {
    Unity::il2cppFieldInfo *m_pField = GetFields(m_pClass, &m_pFieldIterator);
    if (!m_pField)
      break;

    m_pVector->emplace_back(m_pField);
  }
}
Unity::il2cppMethodInfo *IL2CPP::Class::GetMethods(Unity::il2cppClass *m_pClass,
                                                   void **m_pIterator) {
  return reinterpret_cast<Unity::il2cppMethodInfo *(
      IL2CPP_CALLING_CONVENTION)(void *, void **)>(Functions.m_ClassGetMethods)(
      m_pClass, m_pIterator);
}
void IL2CPP::Class::FetchMethods(
    Unity::il2cppClass *m_pClass,
    std::vector<Unity::il2cppMethodInfo *> *m_pVector,
    void *m_pMethodIterator) {
  m_pVector->clear();

  while (1) {
    Unity::il2cppMethodInfo *m_pMethod =
        GetMethods(m_pClass, &m_pMethodIterator);
    if (!m_pMethod)
      break;

    m_pVector->emplace_back(m_pMethod);
  }
}
Unity::il2cppType *IL2CPP::Class::GetType(Unity::il2cppClass *m_pClass) {
  return reinterpret_cast<Unity::il2cppType *(
      IL2CPP_CALLING_CONVENTION)(void *)>(Functions.m_ClassGetType)(m_pClass);
}
Unity::il2cppObject *
IL2CPP::Class::GetSystemType(Unity::il2cppClass *m_pClass) {
  return reinterpret_cast<Unity::il2cppObject *(
      IL2CPP_CALLING_CONVENTION)(void *)>(Functions.m_TypeGetObject)(
      GetType(m_pClass));
}
Unity::il2cppClass *IL2CPP::Class::GetFromName(Unity::il2cppImage *m_pImage,
                                               const char *m_pNamespace,
                                               const char *m_pName) {
  return reinterpret_cast<Unity::il2cppClass *(
      IL2CPP_CALLING_CONVENTION)(void *, const char *, const char *)>(
      Functions.m_ClassFromName)(m_pImage, m_pNamespace, m_pName);
}
Unity::il2cppClass *IL2CPP::Class::Find(const char *m_pName) {
  size_t m_sAssembliesCount = 0U;
  Unity::il2cppAssembly **m_pAssemblies =
      Domain::GetAssemblies(&m_sAssembliesCount);
  if (!m_pAssemblies || 0U >= m_sAssembliesCount)
    return nullptr;

  const char *m_pNameSpaceEnd = strrchr(m_pName, '.');
  char *m_pNameSpace = nullptr;
  if (m_pNameSpaceEnd) {
    uintptr_t m_uNamespaceSize =
        static_cast<uintptr_t>(m_pNameSpaceEnd - m_pName);
    m_pNameSpace = new char[m_uNamespaceSize + 1];
    memcpy(m_pNameSpace, m_pName, m_uNamespaceSize);
    m_pNameSpace[m_uNamespaceSize] = '\0';

    m_pName = m_pNameSpaceEnd + 1;
  } else {
    m_pNameSpace = new char[2];
    memset(m_pNameSpace, 0, 2);
  }

  Unity::il2cppClass *m_pClassReturn = nullptr;
  for (size_t i = 0U; m_sAssembliesCount > i; ++i) {
    Unity::il2cppAssembly *m_pAssembly = m_pAssemblies[i];
    if (!m_pAssembly || !m_pAssembly->m_pImage)
      continue;

    m_pClassReturn = GetFromName(m_pAssembly->m_pImage, m_pNameSpace, m_pName);
    if (m_pClassReturn)
      break;
  }

  delete[] m_pNameSpace;
  return m_pClassReturn;
}
Unity::il2cppObject *IL2CPP::Class::GetSystemType(const char *m_pClassName) {
  Unity::il2cppClass *m_pClass = Find(m_pClassName);
  if (!m_pClass)
    return nullptr;

  return GetSystemType(m_pClass);
}
void IL2CPP::Class::FetchClasses(std::vector<Unity::il2cppClass *> *m_pVector,
                                 const char *m_pModuleName,
                                 const char *m_pNamespace) {
  m_pVector->clear();

  size_t m_sAssembliesCount = 0U;
  Unity::il2cppAssembly **m_pAssemblies =
      Domain::GetAssemblies(&m_sAssembliesCount);
  if (!m_pAssemblies || 0U >= m_sAssembliesCount)
    return;

  Unity::il2cppImage *m_pImage = nullptr;
  for (size_t i = 0U; m_sAssembliesCount > i; ++i) {
    Unity::il2cppAssembly *m_pAssembly = m_pAssemblies[i];
    if (!m_pAssembly || !m_pAssembly->m_pImage ||
        strcmp(m_pAssembly->m_pImage->m_pNameNoExt, m_pModuleName) != 0)
      continue;

    m_pImage = m_pAssembly->m_pImage;
    break;
  }

  if (m_pImage) {
    size_t m_sClassesCount =
        reinterpret_cast<size_t(IL2CPP_CALLING_CONVENTION)(void *)>(
            Functions.m_ImageGetClassCount)(m_pImage);
    for (size_t i = 0U; m_sClassesCount > i; ++i) {
      Unity::il2cppClass *m_pClass = reinterpret_cast<Unity::il2cppClass *(
          IL2CPP_CALLING_CONVENTION)(void *, size_t)>(
          Functions.m_ImageGetClass)(m_pImage, i);
      if (m_pNamespace) {
        if (m_pNamespace[0] == '\0') {
          if (m_pClass->m_pNamespace[0] != '\0')
            continue;
        } else if (strcmp(m_pClass->m_pNamespace, m_pNamespace) != 0)
          continue;
      }

      m_pVector->emplace_back(m_pClass);
    }
  }
}
int IL2CPP::Class::Utils::GetFieldOffset(Unity::il2cppClass *m_pClass,
                                         const char *m_pName) {
  void *m_pFieldIterator = nullptr;
  while (1) {
    Unity::il2cppFieldInfo *m_pField = GetFields(m_pClass, &m_pFieldIterator);
    if (!m_pField)
      break;

    if (strcmp(m_pField->m_pName, m_pName) == 0)
      return m_pField->m_iOffset;
  }

  return -1;
}
int IL2CPP::Class::Utils::GetFieldOffset(const char *m_pClassName,
                                         const char *m_pName) {
  Unity::il2cppClass *m_pClass = Find(m_pClassName);
  if (m_pClass)
    return GetFieldOffset(m_pClass, m_pName);

  return -1;
}
void IL2CPP::Class::Utils::SetStaticField(Unity::il2cppClass *m_pClass,
                                          const char *m_pMemberName,
                                          void *m_pValue) {
  Unity::il2cppFieldInfo *m_pField = reinterpret_cast<Unity::il2cppFieldInfo *(
      IL2CPP_CALLING_CONVENTION)(void *, const char *)>(
      Functions.m_ClassGetFieldFromName)(m_pClass, m_pMemberName);
  if (m_pField)
    reinterpret_cast<void(IL2CPP_CALLING_CONVENTION)(Unity::il2cppFieldInfo *,
                                                     void *)>(
        Functions.m_FieldStaticSetValue)(m_pField, m_pValue);
}
void IL2CPP::Class::Utils::SetStaticField(const char *m_pClassName,
                                          const char *m_pMemberName,
                                          void *m_pValue) {
  Unity::il2cppClass *m_pClass = Find(m_pClassName);
  if (m_pClass)
    SetStaticField(m_pClass, m_pMemberName, m_pValue);
}
void *IL2CPP::Class::Utils::GetStaticField(Unity::il2cppClass *m_pClass,
                                           const char *m_pMemberName) {
  Unity::il2cppFieldInfo *m_pField = reinterpret_cast<Unity::il2cppFieldInfo *(
      IL2CPP_CALLING_CONVENTION)(void *, const char *)>(
      Functions.m_ClassGetFieldFromName)(m_pClass, m_pMemberName);
  void *m_pValue = nullptr;
  if (m_pField)
    reinterpret_cast<void(IL2CPP_CALLING_CONVENTION)(Unity::il2cppFieldInfo *,
                                                     void *)>(
        Functions.m_FieldStaticGetValue)(m_pField, &m_pValue);

  return m_pValue;
}
void *IL2CPP::Class::Utils::GetStaticField(const char *m_pClassName,
                                           const char *m_pMemberName) {
  Unity::il2cppClass *m_pClass = Find(m_pClassName);
  if (m_pClass)
    return GetStaticField(m_pClass, m_pMemberName);

  return nullptr;
}
void *IL2CPP::Class::Utils::GetMethodPointer(Unity::il2cppClass *m_pClass,
                                             const char *m_pMethodName,
                                             int m_iArgs) {
  Unity::il2cppMethodInfo *pMethod = reinterpret_cast<Unity::il2cppMethodInfo *(
      IL2CPP_CALLING_CONVENTION)(void *, const char *, int)>(
      Functions.m_ClassGetMethodFromName)(m_pClass, m_pMethodName, m_iArgs);
  if (!pMethod)
    return nullptr;

  return pMethod->m_pMethodPointer;
}
void *IL2CPP::Class::Utils::GetMethodPointer(const char *m_pClassName,
                                             const char *m_pMethodName,
                                             int m_iArgs) {
  Unity::il2cppClass *m_pClass = Find(m_pClassName);
  if (m_pClass)
    return GetMethodPointer(m_pClass, m_pMethodName, m_iArgs);

  return nullptr;
}
void *IL2CPP::Class::Utils::GetMethodPointer(
    const char *m_pClassName, const char *m_pMethodName,
    std::initializer_list<const char *> m_vNames) {
  Unity::il2cppClass *m_pClass = Find(m_pClassName);
  if (!m_pClass)
    return nullptr;

  int m_iNamesCount = static_cast<int>(m_vNames.size());
  const char **m_pNames = const_cast<const char **>(m_vNames.begin());

  void *m_pMethodIterator = nullptr;
  while (1) {
    Unity::il2cppMethodInfo *m_pMethod =
        GetMethods(m_pClass, &m_pMethodIterator);
    if (!m_pMethod)
      break;

    if (strcmp(m_pMethod->m_pName, m_pMethodName) != 0)
      continue;

#ifdef UNITY_VERSION_2022_3_8F1
    Unity::il2cppType **m_pCurrentParameterTypes = m_pMethod->m_pParameters;

    for (size_t i = 0; i < m_pMethod->m_uArgsCount; ++i) {
      Unity::il2cppType *m_pCurrentParameterType = m_pCurrentParameterTypes[i];
      Unity::il2cppClass *m_pClass = ClassFromType(m_pCurrentParameterType);

      if (strcmp(m_pClass->m_pName, m_pNames[i]) != 0)
        break;

      if ((i + 1) == m_iNamesCount)
        return m_pMethod->m_pMethodPointer;
    }
#else
    Unity::il2cppParameterInfo *m_pCurrentParameters = m_pMethod->m_pParameters;
    for (int i = 0; m_iNamesCount > i; ++i) {
      if (strcmp(m_pCurrentParameters->m_pName, m_pNames[i]) != 0)
        break;

      m_pCurrentParameters++; // m_pCurrentParameters +=
                              // sizeof(Unity::il2cppParameterInfo);
      if ((i + 1) == m_iNamesCount)
        return m_pMethod->m_pMethodPointer;
    }
#endif
  }
  return nullptr;
}
Unity::il2cppClass *
IL2CPP::Class::Utils::ClassFromType(Unity::il2cppType *type) {
  return reinterpret_cast<Unity::il2cppClass *(
      IL2CPP_CALLING_CONVENTION)(void *)>(Functions.m_ClassFromIl2cppType)(
      type);
}
Unity::il2cppType *
IL2CPP::Class::Utils::GetMethodParamType(Unity::il2cppMethodInfo *m_pMethodInfo,
                                         uint32_t index) {
  if (index >= m_pMethodInfo->m_uArgsCount)
    return nullptr;

  return reinterpret_cast<Unity::il2cppType *(
      IL2CPP_CALLING_CONVENTION)(void *, uint32_t)>(Functions.m_MethodGetParam)(
      m_pMethodInfo, index);
}
const char *
IL2CPP::Class::Utils::MethodGetParamName(Unity::il2cppMethodInfo *m_pMethodInfo,
                                         uint32_t index) {
  if (index >= m_pMethodInfo->m_uArgsCount)
    return nullptr;

  return reinterpret_cast<const char *(IL2CPP_CALLING_CONVENTION)(void *,
                                                                  uint32_t)>(
      Functions.m_MethodGetParamName)(m_pMethodInfo, index);
}
Unity::il2cppClass *
IL2CPP::Class::Utils::FilterClass(std::vector<Unity::il2cppClass *> *m_pClasses,
                                  std::initializer_list<const char *> m_vNames,
                                  int m_iFoundCount) {
  int m_iNamesCount = static_cast<int>(m_vNames.size());
  const char **m_pNames = const_cast<const char **>(m_vNames.begin());

  if (0 >= m_iFoundCount || m_iFoundCount > m_iNamesCount)
    m_iFoundCount = m_iNamesCount;

  Unity::il2cppClass *m_pReturn = nullptr;
  for (size_t c = 0; m_pClasses->size() > c; ++c) {
    int m_iFoundCountNow = 0;

    Unity::il2cppClass *m_pClass = m_pClasses->operator[](c);
    if (!m_pClass)
      continue;

    for (int i = 0; m_iNamesCount > i; ++i) {
      const char *m_pNameToFind = m_pNames[i];

      bool m_bFoundInClass = false;
      if (m_pNameToFind[0] == '~') // Field
        m_bFoundInClass = GetFieldOffset(m_pClass, &m_pNameToFind[1]) >= 0;
      else if (m_pNameToFind[0] == '-') // Method
        m_bFoundInClass =
            GetMethodPointer(m_pClass, &m_pNameToFind[1]) != nullptr;
      else // Both
      {
        m_bFoundInClass = GetFieldOffset(m_pClass, m_pNameToFind) >= 0;
        if (!m_bFoundInClass)
          m_bFoundInClass =
              GetMethodPointer(m_pClass, m_pNameToFind) != nullptr;
      }

      if (m_bFoundInClass)
        ++m_iFoundCountNow;
    }

    if (m_iFoundCount == m_iFoundCountNow) {
      m_pReturn = m_pClass;
      break;
    }
  }

  return m_pReturn;
}
void *IL2CPP::Class::Utils::FilterClassToMethodPointer(
    std::vector<Unity::il2cppClass *> *m_pClasses, const char *m_pMethodName,
    int m_iArgs) {
  void *m_pMethodPointer = nullptr;
  for (size_t c = 0; m_pClasses->size() > c; ++c) {
    Unity::il2cppClass *m_pClass = m_pClasses->operator[](c);
    if (!m_pClass)
      continue;

    m_pMethodPointer = GetMethodPointer(m_pClass, m_pMethodName, m_iArgs);
    if (m_pMethodPointer)
      break;
  }

  return m_pMethodPointer;
}
Unity::il2cppFieldInfo *IL2CPP::CClass::GetFields(void **m_pIterator) {
  return Class::GetFields(m_Object.m_pClass, m_pIterator);
}
void IL2CPP::CClass::FetchFields(
    std::vector<Unity::il2cppFieldInfo *> *m_pVector, void *m_pFieldIterator) {
  Class::FetchFields(m_Object.m_pClass, m_pVector, m_pFieldIterator);
}
Unity::il2cppMethodInfo *IL2CPP::CClass::GetMethods(void **m_pIterator) {
  return Class::GetMethods(m_Object.m_pClass, m_pIterator);
}
void IL2CPP::CClass::FetchMethods(
    std::vector<Unity::il2cppMethodInfo *> *m_pVector,
    void *m_pMethodIterator) {
  Class::FetchMethods(m_Object.m_pClass, m_pVector, m_pMethodIterator);
}
void *IL2CPP::CClass::GetMethodPointer(const char *m_pMethodName, int m_iArgs) {
  return Class::Utils::GetMethodPointer(m_Object.m_pClass, m_pMethodName,
                                        m_iArgs);
}
IL2CPP::m_eClassPropType IL2CPP::CClass::GetPropType(const char *m_pPropType) {
  Unity::il2cppFieldInfo *pField = reinterpret_cast<Unity::il2cppFieldInfo *(
      IL2CPP_CALLING_CONVENTION)(void *, const char *)>(
      Functions.m_ClassGetFieldFromName)(m_Object.m_pClass, m_pPropType);
  if (pField)
    return m_eClassPropType::Field;

  Unity::il2cppPropertyInfo *pProperty =
      reinterpret_cast<Unity::il2cppPropertyInfo *(
          IL2CPP_CALLING_CONVENTION)(void *, const char *)>(
          Functions.m_ClassGetPropertyFromName)(m_Object.m_pClass, m_pPropType);
  if (pProperty)
    return m_eClassPropType::Property;

  Unity::il2cppMethodInfo *pMethod = reinterpret_cast<Unity::il2cppMethodInfo *(
      IL2CPP_CALLING_CONVENTION)(void *, const char *, int)>(
      Functions.m_ClassGetMethodFromName)(m_Object.m_pClass, m_pPropType, -1);
  if (pMethod)
    return m_eClassPropType::Method;

  return m_eClassPropType::Unknown;
}
Unity::il2cppClass *IL2CPP::CClass::GetIl2cppClass() const {
  if (m_Object.m_pClass == nullptr) {
    return nullptr;
  }
  return m_Object.m_pClass;
}
std::string IL2CPP::CClass::GetType() const {
  if (m_Object.m_pClass == nullptr) {
    return "";
  }
  return fmt::format("{}.{}", m_Object.m_pClass->m_pNamespace,
                     m_Object.m_pClass->m_pName);
}
bool IL2CPP::CClass::Is(Unity::il2cppClass *m_pClass) const {
  if (m_Object.m_pClass == m_pClass) {
    return true;
  }
  return false;
}
