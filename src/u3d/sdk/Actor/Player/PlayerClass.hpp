#pragma once

#include "IL2CPPResolver/Unity/API/Component.hpp"
#include "IL2CPPResolver/Unity/API/Object.hpp"
#include <string>
namespace zr {
class Player;
class PlayerClass : public Unity::CObject {
public:
  Player *getPlayer();
  Unity::CGameObject *getPlayerModel();
  std::string getName();
  std::string getHand();
  std::string getTeamID();
  std::string getClassColor();
};
} // namespace zr