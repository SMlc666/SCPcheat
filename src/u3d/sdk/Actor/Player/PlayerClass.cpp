#include "PlayerClass.hpp"
#include "IL2CPPResolver/Unity/Structures/System_String.hpp"
#include "u3d/sdk/Actor/Player/Player.hpp"

namespace zr {
Player *PlayerClass::getPlayer() { return GetMemberValue<Player *>("player"); }
Unity::CGameObject *PlayerClass::getPlayerModel() {
  return GetMemberValue<Unity::CGameObject *>("playerModel");
}
std::string PlayerClass::getName() {
  return CallMethod<Unity::System_String *>("GetName")->ToString();
}
std::string PlayerClass::getHand() {
  return CallMethod<Unity::System_String *>("GetHand")->ToString();
}
std::string PlayerClass::getTeamID() {
  return CallMethod<Unity::System_String *>("GetTeamID")->ToString();
}
std::string PlayerClass::getClassColor() {
  return CallMethod<Unity::System_String *>("GetClassColor")->ToString();
}
} // namespace zr