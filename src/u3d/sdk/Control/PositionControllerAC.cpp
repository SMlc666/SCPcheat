#include "PositionControllerAC.hpp"
#include "IL2CPPResolver/Unity/Structures/Engine.hpp"

namespace zr {
Unity::Vector3 PositionControllerAC::getPrevPosition() {
  return GetMemberValue<Unity::Vector3>("prevPos");
}
} // namespace zr