#pragma once

#include "IL2CPPResolver/Unity/API/Component.hpp"
#include "IL2CPPResolver/Unity/Structures/Engine.hpp"
#include <cstdint>
namespace zr {
class Player : public Unity::CComponent {
public:
  Player() = delete;

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
};
} // namespace zr