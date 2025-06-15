#include "Shader.hpp"
#include "IL2CPPResolver/API/Class.hpp"
#include "IL2CPPResolver/API/String.hpp"
#include "IL2CPPResolver/Unity/Structures/System_String.hpp"

namespace zr {

Shader *Shader::Find(const std::string &name) {
  static void *methodPtr =
      IL2CPP::Class::Utils::GetMethodPointer("UnityEngine.Shader", "Find");
  Shader *(UNITY_CALLING_CONVENTION t)(Unity::System_String *) = nullptr;
  return reinterpret_cast<decltype(t)>(methodPtr)(IL2CPP::String::New(name));
}

} // namespace zr
