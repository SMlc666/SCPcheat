#pragma once


#include <Windows.h>
namespace IL2CPP {
namespace Thread {
void *Attach(void *m_Domain);

void Detach(void *m_Thread);
} // namespace Thread

// Our Stuff
class CThread {
public:
  void *m_OnStart = nullptr;
  void *m_OnEnd = nullptr;

  static DWORD __stdcall Handler(void *m_Reserved);

  CThread();
  CThread(void *m_OnStartFunc, void *m_OnEndFunc);
};

namespace Thread {
void Create(void *m_OnStartFunc, void *m_OnEndFunc = nullptr);
} // namespace Thread
} // namespace IL2CPP