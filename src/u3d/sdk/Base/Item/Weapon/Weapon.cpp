#include "Weapon.hpp"
#include "IL2CPPResolver/API/Class.hpp"
#include <cstdint>

namespace zr {
int32_t Weapon::getShootLayer() {
  static void *pFunc = IL2CPP::Class::Utils::GetMethodPointer(
      "UnityEngine.LayerMask", "op_Implicit", 1);

  return reinterpret_cast<int32_t(UNITY_CALLING_CONVENTION)(int32_t)>(pFunc)(
      GetMemberValue<int32_t>("shootLayer"));
}
} // namespace zr