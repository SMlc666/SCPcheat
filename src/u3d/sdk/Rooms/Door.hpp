#pragma once
#include "IL2CPPResolver/Unity/API/Component.hpp"
#include "IL2CPPResolver/Unity/Structures/Vector3.hpp"
namespace zr {
class Door : public Unity::CComponent {
public:
  bool getLockedByAP();
  void setLockedByAP(bool locked);
  void forceState(bool state);
  bool getOpened();
  bool getCanBreak();
  bool getBroken();
  void breakDoor(Unity::Vector3 pos, bool ignoreContDoor);
  void breakDoorClient(Unity::Vector3 pos);
};
} // namespace zr