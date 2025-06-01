#pragma once

#include <vector>
namespace IL2CPP {
struct CallbackHook_t {
  std::vector<void *> m_Funcs;

  void **m_VFunc = nullptr;
  void *m_Original = nullptr;
};

namespace Callback {
namespace OnUpdate {
extern CallbackHook_t m_CallbackHook;

void Add(void *m_pFunction);

void __fastcall Hook(void *rcx);
} // namespace OnUpdate

namespace OnLateUpdate {
extern CallbackHook_t m_CallbackHook;

void Add(void *m_pFunction);

void __fastcall Hook(void *rcx);
} // namespace OnLateUpdate

void Initialize();

void Uninitialize();
} // namespace Callback
} // namespace IL2CPP