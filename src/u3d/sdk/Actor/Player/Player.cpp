#include "Player.hpp"
#include "IL2CPPResolver/Unity/API/GameObject.hpp"
#include "IL2CPPResolver/Unity/API/Object.hpp"
#include "IL2CPPResolver/Unity/Structures/Engine.hpp"
#include "IL2CPPResolver/Unity/Structures/System_String.hpp"
#include "u3d/sdk/Actor/Collider/CapsuleCollider.hpp"
#include <cstdint>
#include <vector>

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
zr::PlayerClass *zr::Player::getPlayerClass() {
  return GetMemberValue<zr::PlayerClass *>("playerClass");
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
zr::FirstPersonController *zr::Player::getFirstPersonController() {
  return GetMemberValue<zr::FirstPersonController *>("fps");
}
zr::PositionControllerAC *zr::Player::getPositionControllerAC() {
  return GetMemberValue<zr::PositionControllerAC *>("positionControllerAC");
}
std::vector<zr::Player *> zr::Player::getAllPlayers() {
  const auto players = Unity::Object::FindObjectsOfType<Player>("Player");
  if (!players || players->m_uMaxLength <= 0) {
    return {};
  }

  std::vector<zr::Player *> validPlayers;
  validPlayers.reserve(players->m_uMaxLength);

  for (int i = 0; i < players->m_uMaxLength; ++i) {
    const auto player = players->At(i);
    if (player && player->isInit()) { // 保持原始调用方式
      validPlayers.push_back(player);
    }
  }

  if (validPlayers.capacity() > validPlayers.size() * 2) {
    validPlayers.shrink_to_fit();
  }

  return validPlayers;
}
zr::CapsuleCollider *zr::Player::getCapsuleCollider() {
  return static_cast<CapsuleCollider *>(
      GetGameObject()->GetComponent("UnityEngine.CapsuleCollider"));
}
