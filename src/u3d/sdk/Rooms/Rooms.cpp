#include "Rooms.hpp"
#include "IL2CPPResolver/Unity/API/Object.hpp"

namespace zr {
std::vector<Rooms *> Rooms::getAll() {
  const auto rooms = Unity::Object::FindObjectsOfType<Rooms>("Rooms");
  if (!rooms || rooms->m_uMaxLength <= 0) {
    return {};
  }

  std::vector<zr::Rooms *> validRooms;
  validRooms.reserve(rooms->m_uMaxLength);

  for (int i = 0; i < rooms->m_uMaxLength; ++i) {
    const auto room = rooms->At(i);
    if (room) { // 保持原始调用方式
      validRooms.push_back(room);
    }
  }

  if (validRooms.capacity() > validRooms.size() * 2) {
    validRooms.shrink_to_fit();
  }

  return validRooms;
};
std::string Rooms::getName() {
  return GetMemberValue<Unity::System_String *>("roomName")->ToString();
}
} // namespace zr