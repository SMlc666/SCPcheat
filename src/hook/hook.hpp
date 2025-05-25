#pragma once
#include <windows.h>

#include <detours.h>

namespace zr {

class Hook {
private:
  void *m_func;
  void *m_hook;

  bool m_attached = false;

private: // Private methods
  void install();
  void uninstall();

public:
  Hook() = default;
  template <typename T, typename U>
  Hook(T *func, U *hook)
      : m_func(reinterpret_cast<void *>(func)),
        m_hook(reinterpret_cast<void *>(hook)) {

    install();
  }
  ~Hook();

  template <typename T, typename... Args> T call(Args... args) {
    return reinterpret_cast<T (*)(Args...)>(m_func)(args...);
  }
  template <typename T, typename... Args> T stdcall(Args... args) {
    return reinterpret_cast<T(__stdcall *)(Args...)>(m_func)(args...);
  }
  template <typename T, typename... Args> T fastcall(Args... args) {
    return reinterpret_cast<T(__fastcall *)(Args...)>(m_func)(args...);
  }
};

} // namespace zr