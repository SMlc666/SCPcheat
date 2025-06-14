#include "FirstPersonController.hpp"

namespace zr {
bool FirstPersonController::isJump() { return GetMemberValue<bool>("m_Jump"); }
bool FirstPersonController::isJumping() {
  return GetMemberValue<bool>("m_Jumping");
}
bool FirstPersonController::isWalking() {
  return GetMemberValue<bool>("m_IsWalking");
}
void FirstPersonController::setJump(bool jump) {
  SetMemberValue("m_Jump", jump);
}
void FirstPersonController::setJumping(bool jumping) {
  SetMemberValue("m_Jumping", jumping);
}
void FirstPersonController::setWalking(bool walking) {
  SetMemberValue("m_IsWalking", walking);
}
MouseLook *FirstPersonController::getMouseLook() {
  return GetMemberValue<MouseLook *>("m_MouseLook");
}
} // namespace zr