#include "Callback.hpp"
#include "Domain.hpp"
#include "IL2CPPResolver/API/Thread.hpp"
#include "IL2CPPResolver/Utils/Helper.hpp"
#include "IL2CPPResolver/Utils/VTable.hpp"
IL2CPP::CallbackHook_t IL2CPP::Callback::OnUpdate::m_CallbackHook;
IL2CPP::CallbackHook_t IL2CPP::Callback::OnLateUpdate::m_CallbackHook;
void IL2CPP::Callback::Uninitialize() {
  Utils::VTable::ReplaceFunction(OnUpdate::m_CallbackHook.m_VFunc,
                                 OnUpdate::m_CallbackHook.m_Original);
  Utils::VTable::ReplaceFunction(OnLateUpdate::m_CallbackHook.m_VFunc,
                                 OnLateUpdate::m_CallbackHook.m_Original);
}
void IL2CPP::Callback::Initialize() {
  void *m_IL2CPPThread = Thread::Attach(IL2CPP::Domain::Get());

  // Find
  void **m_MonoBehaviourVTable = *reinterpret_cast<void ***>(
      IL2CPP::Helper::GetMonoBehaviour()->m_CachedPtr);
  if (m_MonoBehaviourVTable) {
#ifdef _WIN64
    OnUpdate::m_CallbackHook.m_VFunc = Utils::VTable::FindFunction(
        m_MonoBehaviourVTable, 99, {0x33, 0xD2, 0xE9}); // xor edx, edx | jmp
    OnLateUpdate::m_CallbackHook.m_VFunc = Utils::VTable::FindFunction(
        m_MonoBehaviourVTable, 99,
        {0xBA, 0x01, 0x00, 0x00, 0x00, 0xE9}); //  mov edx, 1 | jmp
#elif _WIN32
    OnUpdate::m_CallbackHook.m_VFunc = Utils::VTable::FindFunction(
        m_MonoBehaviourVTable, 99, {0x6A, 0x00, 0xE8}); // push 00 | call
    OnLateUpdate::m_CallbackHook.m_VFunc = Utils::VTable::FindFunction(
        m_MonoBehaviourVTable, 99, {0x6A, 0x01, 0xE8}); // push 01 | call
#endif
  }

  IL2CPP::Thread::Detach(m_IL2CPPThread);

  // Replace (Hook)
  Utils::VTable::ReplaceFunction(OnUpdate::m_CallbackHook.m_VFunc,
                                 OnUpdate::Hook,
                                 &OnUpdate::m_CallbackHook.m_Original);
  Utils::VTable::ReplaceFunction(OnLateUpdate::m_CallbackHook.m_VFunc,
                                 OnLateUpdate::Hook,
                                 &OnLateUpdate::m_CallbackHook.m_Original);
}
void __fastcall IL2CPP::Callback::OnLateUpdate::Hook(void *rcx) {
  for (void *m_Func : m_CallbackHook.m_Funcs)
    reinterpret_cast<void (*)()>(m_Func)();

  reinterpret_cast<void(__fastcall *)(void *)>(m_CallbackHook.m_Original)(rcx);
}
void IL2CPP::Callback::OnLateUpdate::Add(void *m_pFunction) {
  m_CallbackHook.m_Funcs.emplace_back(m_pFunction);
}
void __fastcall IL2CPP::Callback::OnUpdate::Hook(void *rcx) {
  for (void *m_Func : m_CallbackHook.m_Funcs)
    reinterpret_cast<void (*)()>(m_Func)();

  reinterpret_cast<void(__fastcall *)(void *)>(m_CallbackHook.m_Original)(rcx);
}
void IL2CPP::Callback::OnUpdate::Add(void *m_pFunction) {
  m_CallbackHook.m_Funcs.emplace_back(m_pFunction);
}
