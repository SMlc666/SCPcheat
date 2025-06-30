#include "PlayerClass.hpp"
#include "IL2CPPResolver/API/String.hpp"
#include "IL2CPPResolver/Unity/Structures/System_String.hpp"
#include "log/log.hpp"
#include "u3d/sdk/Actor/Player/Player.hpp"

namespace zr {
Player *PlayerClass::getPlayer() { return GetMemberValue<Player *>("player"); }
Unity::CGameObject *PlayerClass::getPlayerModel() {
  return GetMemberValue<Unity::CGameObject *>("playerModel");
}
std::string PlayerClass::getName() {
  return CallMethod<Unity::System_String *>("GetName")->ToString();
}
std::string PlayerClass::getHand() {
  return CallMethod<Unity::System_String *>("GetHand")->ToString();
}
std::string PlayerClass::getTeamID() {
  return CallMethod<Unity::System_String *>("GetTeamID")->ToString();
}
std::string PlayerClass::getClassColor() {
  return CallMethod<Unity::System_String *>("GetClassColor")->ToString();
}
void PlayerClass::sendToServer(const std::string &func,
                               const std::vector<Unity::il2cppObject *> &args) {
  static void *sendToServerPtr;
  if (!sendToServerPtr) {
    sendToServerPtr = IL2CPP::Class::Utils::GetMethodPointer(
        "Akequ.Base.PlayerClass", "SendToServer", 2);
    if (!sendToServerPtr) {
      zr::LogInstance::getMainLogger().error("Can't find SendToServer method");
    }
  }
  static Unity::il2cppClass *systemObjectClass;
  if (!systemObjectClass) {
    systemObjectClass = IL2CPP::Class::Find("System.Object");
    if (!systemObjectClass) {
      zr::LogInstance::getMainLogger().error("Can't find System.Object class");
    }
  }
  (reinterpret_cast<void(UNITY_CALLING_CONVENTION)(
       PlayerClass *, Unity::System_String *,
       Unity::il2cppArray<Unity::il2cppObject *> *)>(sendToServerPtr))(
      this, IL2CPP::String::New(func),
      Unity::il2cppArray<Unity::il2cppObject *>::Create(systemObjectClass,
                                                        args));
}
} // namespace zr