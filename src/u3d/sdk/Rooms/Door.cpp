#include "Door.hpp"
#include "IL2CPPResolver/Unity/Structures/Vector3.hpp"

namespace zr {
bool Door::getLockedByAP() { return GetMemberValue<bool>("lockedByAP"); }
void Door::setLockedByAP(bool value) { SetMemberValue("lockedByAP", value); }
void Door::forceState(bool state) { CallMethod<void>("ForceState", state); }
bool Door::getOpened() { return GetMemberValue<bool>("opened"); }
bool Door::getCanBreak() { return GetMemberValue<bool>("canBreak"); }
bool Door::getBroken() { return GetMemberValue<bool>("broken"); }
void Door::breakDoor(Unity::Vector3 pos, bool ignoreContDoor) {
  return CallMethod<void>("BreakDoor", pos, ignoreContDoor);
}
void Door::breakDoorClient(Unity::Vector3 pos) {
  return CallMethod<void>("BreakDoorClient", pos);
}
} // namespace zr