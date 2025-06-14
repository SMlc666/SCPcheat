#include "Item.hpp"

namespace zr {
Player *Item::getPlayer() { return GetMemberValue<Player *>(0x10); }
} // namespace zr