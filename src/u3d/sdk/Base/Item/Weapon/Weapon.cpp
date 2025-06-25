#include "Weapon.hpp"

namespace zr {
int32_t Weapon::getShootLayer() {
  return GetMemberValue<int32_t>("shootLayer");
}
} // namespace zr