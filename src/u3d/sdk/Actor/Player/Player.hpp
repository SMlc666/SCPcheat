#pragma once

#include "IL2CPPResolver/Unity/API/Component.hpp"
#include "IL2CPPResolver/Unity/Structures/Engine.hpp"
#include "u3d/sdk/Actor/Collider/CapsuleCollider.hpp"
#include "u3d/sdk/Actor/Player/PlayerClass.hpp"
#include "u3d/sdk/Base/AmmoData.hpp"
#include "u3d/sdk/Control/FirstPersonController.hpp"
#include "u3d/sdk/Control/PositionControllerAC.hpp"
#include <cstdint>

namespace zr {
class Player : public Unity::CComponent {
public:
  Player() = delete;
  PlayerClass *getPlayerClass();
  static std::vector<Player *> getAllPlayers();
  void setSpeed(float walkSpeed, float runSpeed);
  void setJumpPower(float power);
  void teleport(Unity::Vector3 vec);
  void teleportClient(Unity::Vector3 vec);
  std::string getClassName();
  std::string getAccountName();
  uint32_t getHealth();
  uint32_t getMaxHealth();
  bool isInit();
  bool isAdmin();
  std::vector<AmmoData *> getAmmoData();
  FirstPersonController *getFirstPersonController();
  PositionControllerAC *getPositionControllerAC();
  CapsuleCollider *getCapsuleCollider();
  int16_t getPlayerID();
  void syncAccount(int64_t id, const std::string &name, const std::string &tag,
                   const std::string &platform);
};
} // namespace zr