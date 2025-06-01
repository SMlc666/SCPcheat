#include "ResolveCall.hpp"
#include "IL2CPPResolver/Data.hpp"
#include "IL2CPPResolver/Defines.hpp"
void *IL2CPP::ResolveCall(const char *m_Name) {
  return reinterpret_cast<void *(IL2CPP_CALLING_CONVENTION)(const char *)>(
      Functions.m_ResolveFunction)(m_Name);
}
