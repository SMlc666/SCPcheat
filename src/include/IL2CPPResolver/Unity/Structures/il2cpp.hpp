#pragma once

#include "IL2CPPResolver/Data.hpp"
#include "IL2CPPResolver/Defines.hpp"
#include <cstdint>
namespace Unity {
struct il2cppAssembly;
struct il2cppImage {
  const char *m_pName;
  const char *m_pNameNoExt;
  il2cppAssembly *m_pAssembly;
  uint32_t m_uTypeCount;
};

struct il2cppAssemblyName {
  const char *m_pName;
  const char *m_pCulture;
  const char *m_pHash;
  const char *m_pPublicKey;
  unsigned int m_uHash;
  int m_iHashLength;
  unsigned int m_uFlags;
  int m_iMajor;
  int m_iMinor;
  int m_iBuild;
  int m_bRevision;
  unsigned char m_uPublicKeyToken[8];
};

struct il2cppAssembly {
  il2cppImage *m_pImage;
  unsigned int m_uToken;
  int m_ReferencedAssemblyStart;
  int m_ReferencedAssemblyCount;
  il2cppAssemblyName m_aName;
};

struct il2cppClass {
  void *m_pImage;
  void *m_pGC;
  const char *m_pName;
  const char *m_pNamespace;
  void *m_pValue;
  void *m_pArgs;
  il2cppClass *m_pElementClass;
  il2cppClass *m_pCastClass;
  il2cppClass *m_pDeclareClass;
  il2cppClass *m_pParentClass;
  void *m_pGenericClass;
  void *m_pTypeDefinition;
  void *m_pInteropData;
  void *m_pFields;
  void *m_pEvents;
  void *m_pProperties;
  void **m_pMethods;
  il2cppClass **m_pNestedTypes;
  il2cppClass **m_ImplementedInterfaces;
  void *m_pInterfaceOffsets;
  void *m_pStaticFields;
  void *m_pRGCTX;
  struct Il2CppClass **typeHierarchy;
  void *unity_user_data;
  void *initializationExceptionGCHandle;
  uint32_t cctor_started;
  uint32_t cctor_finished_or_no_cctor;
  __declspec(align(8)) size_t cctor_thread;
  void *genericContainerHandle;
  uint32_t instance_size;
  uint32_t stack_slot_size;
  uint32_t actualSize;
  uint32_t element_size;
  int32_t native_size;
  uint32_t static_fields_size;
  uint32_t thread_static_fields_size;
  int32_t thread_static_fields_offset;
  uint32_t flags;

  inline void* NewArray(uintptr_t size) {
    return reinterpret_cast<void*(IL2CPP_CALLING_CONVENTION)(il2cppClass*, uintptr_t)>(IL2CPP::Functions.m_ArrayNew)(this, size);
  }
};

struct il2cppObject {
  union {
    il2cppClass *m_pClass;
    void *m_pVtable;
  };
  void *m_pMonitor;
  /**
        @brief Unbox any object.
        @return Unboxed object of passed type
    */
  template <typename T> inline T UnboxObject(T obj) {
    return (T)(void *)(((char *)obj) + sizeof(il2cppObject));
  }
  static il2cppObject *BoxObject(il2cppClass *_data, void *data) {
    return reinterpret_cast<il2cppObject *(
        IL2CPP_CALLING_CONVENTION)(il2cppClass *, void *)>(
        IL2CPP::Functions.m_ValueBox)(_data, data);
  }
};

#ifdef UNITY_VERSION_2022_3_8F1
struct il2cppType {
  void *data;
  unsigned int bits;
};
#else
struct il2cppType {
  union {
    void *m_pDummy;
    unsigned int m_uClassIndex;
    il2cppType *m_pType;
    void *m_pArray;
    unsigned int m_uGenericParameterIndex;
    void *m_pGenericClass;
  };
  unsigned int m_uAttributes : 16;
  unsigned int m_uType : 8;
  unsigned int m_uMods : 6;
  unsigned int m_uByref : 1;
  unsigned int m_uPinned : 1;
};
#endif

struct il2cppFieldInfo {
  const char *m_pName;
  il2cppType *m_pType;
  il2cppClass *m_pParentClass;
  int m_iOffset;
  int m_iAttributeIndex;
  unsigned int m_uToken;
};

struct il2cppParameterInfo {
  const char *m_pName;
  int m_iPosition;
  unsigned int m_uToken;
  il2cppType *m_pParameterType;
};

struct il2cppMethodInfo {
  void *m_pMethodPointer;
#ifdef UNITY_VERSION_2022_3_8F1
  void *m_pVirtualMethodPointer;
#endif
  void *m_pInvokerMethod;
  const char *m_pName;
  il2cppClass *m_pClass;
  il2cppType *m_pReturnType;
#ifdef UNITY_VERSION_2022_3_8F1
  il2cppType **m_pParameters;
#else
  il2cppParameterInfo *m_pParameters;
#endif

  union {
    void *m_pRGCTX;
    void *m_pMethodDefinition;
  };

  union {
    void *m_pGenericMethod;
    void *m_pGenericContainer;
  };

  unsigned int m_uToken;
  unsigned short m_uFlags;
  unsigned short m_uFlags2;
  unsigned short m_uSlot;
  unsigned char m_uArgsCount;
  unsigned char m_uGeneric : 1;
  unsigned char m_uInflated : 1;
  unsigned char m_uWrapperType : 1;
  unsigned char m_uMarshaledFromNative : 1;
};

struct il2cppPropertyInfo {
  il2cppClass *m_pParentClass;
  const char *m_pName;
  il2cppMethodInfo *m_pGet;
  il2cppMethodInfo *m_pSet;
  unsigned int m_uAttributes;
  unsigned int m_uToken;
};

struct il2cppArrayBounds {
  uintptr_t m_uLength;
  int m_iLowerBound;
};
} // namespace Unity
