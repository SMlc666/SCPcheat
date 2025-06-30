#pragma once
#include "IL2CPPResolver/Unity/API/Object.hpp"
#include "IL2CPPResolver/Unity/Structures/il2cpp.hpp"
#include "u3d/sdk/Actor/Player/Player.hpp"
namespace zr {
class Item : public Unity::CObject {
public:
  Player *getPlayer();
  void sendToServer(const std::string &func,
                    const std::vector<Unity::il2cppObject *> &args);
};
} // namespace zr