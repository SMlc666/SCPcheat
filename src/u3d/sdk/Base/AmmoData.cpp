#include "AmmoData.hpp"
#include "IL2CPPResolver/Unity/Structures/System_String.hpp"

namespace zr {
std::string AmmoData::getAmmoType() {
  auto str = GetMemberValue<Unity::System_String *>("ammoType");
  if (str) {
    return str->ToString();
  }
  return {};
}
uint16_t AmmoData::getCurrent() { return GetMemberValue<uint16_t>("current"); }
uint16_t AmmoData::getMaxAmmo() { return GetMemberValue<uint16_t>("maxAmmo"); }
std::string AmmoData::getUI() {
  auto str = GetMemberValue<Unity::System_String *>("ui");
  if (str) {
    return str->ToString();
  }
  return {};
}
} // namespace zr