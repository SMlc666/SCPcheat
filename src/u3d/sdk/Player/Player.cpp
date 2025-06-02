#include "Player.hpp"
#include "IL2CPPResolver/Unity/Structures/Engine.hpp"
#include "IL2CPPResolver/Unity/Structures/System_String.hpp"
#include <cstdint>

void zr::Player::setSpeed(float walkSpeed, float runSpeed) {
  CallMethod<void>("SetSpeed", walkSpeed, runSpeed);
}
void zr::Player::setJumpPower(float power) {
  CallMethod<void>("SetJumpPower", power);
}
void zr::Player::teleport(Unity::Vector3 vec) {
  CallMethod<void>("Teleport", vec);
}
void zr::Player::teleportClient(Unity::Vector3 vec) {
  CallMethod<void>("TeleportClient", vec);
}
uint32_t zr::Player::getHealth() { return GetMemberValue<uint32_t>("health"); }
uint32_t zr::Player::getMaxHealth() {
  return GetMemberValue<uint32_t>("maxHealth");
}
std::string zr::Player::getClassName() {
  return GetMemberValue<Unity::System_String *>("_classname")->ToString();
}
std::string zr::Player::getAccountName() {
  return GetMemberValue<Unity::System_String *>("accountName")->ToString();
}
bool zr::Player::isInit() { return GetMemberValue<bool>("isPlayerInit"); }
bool zr::Player::isAdmin() { return GetMemberValue<bool>("isAdmin"); }