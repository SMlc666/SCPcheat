#include "SystemTypeCache.hpp"

std::unordered_map<uint32_t, Unity::il2cppObject *>
    IL2CPP::SystemTypeCache::m_Map;
std::vector<const char *> IL2CPP::SystemTypeCache::Initializer::m_List;
void IL2CPP::SystemTypeCache::Add(uint32_t m_Hash,
                                  Unity::il2cppObject *m_SystemType) {
  m_Map[m_Hash] = m_SystemType;
}
void IL2CPP::SystemTypeCache::Add(const char *m_Name,
                                  Unity::il2cppObject *m_SystemType) {
  Add(Utils::Hash::Get(m_Name), m_SystemType);
}
Unity::il2cppObject *IL2CPP::SystemTypeCache::Get(uint32_t m_Hash) {
  return m_Map[m_Hash];
}
Unity::il2cppObject *IL2CPP::SystemTypeCache::Get(const char *m_Name) {
  return Get(Utils::Hash::Get(m_Name));
}
Unity::il2cppObject *IL2CPP::SystemTypeCache::Find(uint32_t m_Hash) {
  return Get(m_Hash);
}
Unity::il2cppObject *IL2CPP::SystemTypeCache::Find(const char *m_Name) {
  return Get(m_Name);
}
void IL2CPP::SystemTypeCache::Initializer::Add(const char *m_Name) {
  m_List.emplace_back(m_Name);
}
void IL2CPP::SystemTypeCache::Initializer::PreCache() {
  for (const char *m_Name : m_List)
    SystemTypeCache::Add(m_Name, IL2CPP::Class::GetSystemType(m_Name));

  m_List.clear();
}
