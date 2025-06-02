#pragma once
#include "Player.hpp"

namespace zr {
class LocalPlayer : public Player {
public:
  static LocalPlayer *getInstance();
};
} // namespace zr