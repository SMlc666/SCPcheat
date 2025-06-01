#pragma once

#include "IL2CPPResolver/Unity/Structures/il2cpp.hpp"
namespace IL2CPP {
namespace Domain {
void *Get();
Unity::il2cppAssembly **GetAssemblies(size_t *m_Size);
} // namespace Domain
} // namespace IL2CPP