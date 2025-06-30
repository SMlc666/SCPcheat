#pragma once

#include "IL2CPPResolver/Unity/API/Component.hpp"
#include "u3d/sdk/Actor/Player/Player.hpp"
namespace zr {
class HitBox : public Unity::CComponent {
public:
  Player *getPlayer();
};
} // namespace zr