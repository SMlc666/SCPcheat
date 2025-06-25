#pragma once
#include "IL2CPPResolver/Unity/API/Object.hpp"
#include "u3d/sdk/Actor/Player/Player.hpp"
namespace zr {
class Item : public Unity::CObject {
public:
  Player *getPlayer();
  
};
} // namespace zr