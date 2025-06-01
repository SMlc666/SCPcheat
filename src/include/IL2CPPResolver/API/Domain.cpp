#include "Domain.hpp"
#include "IL2CPPResolver/Data.hpp"
#include "IL2CPPResolver/Defines.hpp"
Unity::il2cppAssembly **IL2CPP::Domain::GetAssemblies(size_t *m_Size) {
  return reinterpret_cast<Unity::il2cppAssembly **(
      IL2CPP_CALLING_CONVENTION)(void *, size_t *)>(
      Functions.m_DomainGetAssemblies)(Get(), m_Size);
}
void *IL2CPP::Domain::Get() {
  return reinterpret_cast<void *(IL2CPP_CALLING_CONVENTION)()>(
      Functions.m_DomainGet)();
}
