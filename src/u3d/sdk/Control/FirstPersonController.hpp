#pragma once

#include "IL2CPPResolver/Unity/API/Component.hpp"
namespace zr {
class FirstPersonController : public Unity::CComponent {
public:
  bool isJump();
  bool isJumping();
  bool isWalking();
  void setJump(bool jump);
  void setJumping(bool jumping);
  void setWalking(bool walking);
};
} // namespace zr