#pragma once

#include "IL2CPPResolver/Unity/Structures/System_String.hpp"
#include "u3d/sdk/Base/ValueType.hpp"
#include <cstddef>
#include <cstdint>
namespace zr {
struct AmmoData : public ValueType {
  std::string getAmmoType();
  uint16_t getCurrent();
  uint16_t getMaxAmmo();
  std::string getUI();
};

} // namespace zr