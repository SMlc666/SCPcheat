#pragma once

#include "IL2CPPResolver/Unity/Structures/il2cpp.hpp"
#include <Windows.h>
#include <string>

namespace Unity {
struct System_String : il2cppObject {
  int m_iLength;
  wchar_t m_wString[1024];

  void Clear();

  std::string ToString();
};
} // namespace Unity
