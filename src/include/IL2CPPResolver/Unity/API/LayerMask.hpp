#pragma once

#include "IL2CPPResolver/Unity/Structures/System_String.hpp"
namespace Unity {
struct LayerMaskFunctions_t {
  void *m_LayerToName = nullptr;
  void *m_NameToLayer = nullptr;
};
extern LayerMaskFunctions_t m_LayerMaskFunctions;

namespace LayerMask {
void Initialize();

System_String *LayerToName(unsigned int m_uLayer);

uint32_t NameToLayer(const char *m_pName);
} // namespace LayerMask
} // namespace Unity