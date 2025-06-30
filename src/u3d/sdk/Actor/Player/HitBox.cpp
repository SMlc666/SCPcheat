#include "HitBox.hpp"
namespace zr {
Player *HitBox::getPlayer() { return GetMemberValue<Player *>("player"); }
} // namespace zr