#pragma once

#include "IL2CPPResolver/API/Class.hpp"
#include "IL2CPPResolver/Unity/Structures/il2cpp.hpp"
#include "Utils/Hash.hpp"
#include <cstdint>
#include <unordered_map>
namespace IL2CPP {
namespace SystemTypeCache {
extern std::unordered_map<uint32_t, Unity::il2cppObject *> m_Map;

void Add(uint32_t m_Hash, Unity::il2cppObject *m_SystemType);

void Add(const char *m_Name, Unity::il2cppObject *m_SystemType);

Unity::il2cppObject *Get(uint32_t m_Hash);

Unity::il2cppObject *Get(const char *m_Name);

// Legacy Naming
Unity::il2cppObject *Find(uint32_t m_Hash);

Unity::il2cppObject *Find(const char *m_Name);

namespace Initializer {
extern std::vector<const char *> m_List;

void Add(const char *m_Name);

void PreCache();
} // namespace Initializer
} // namespace SystemTypeCache
} // namespace IL2CPP