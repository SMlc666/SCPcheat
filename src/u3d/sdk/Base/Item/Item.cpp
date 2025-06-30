#include "Item.hpp"
#include "IL2CPPResolver/API/Class.hpp"
#include "IL2CPPResolver/API/String.hpp"
#include "IL2CPPResolver/Unity/API/Object.hpp"
#include "IL2CPPResolver/Unity/Structures/System_String.hpp"
#include "IL2CPPResolver/Unity/Structures/il2cpp.hpp"
#include "IL2CPPResolver/Unity/Structures/il2cppArray.hpp"
#include "log/log.hpp"

namespace zr {
Player *Item::getPlayer() { return GetMemberValue<Player *>(0x10); }
void Item::sendToServer(const std::string &func,
                        const std::vector<Unity::il2cppObject *> &args) {
  static void *sendToServerPtr;
  if (!sendToServerPtr) {
    sendToServerPtr = IL2CPP::Class::Utils::GetMethodPointer("Akequ.Base.Item",
                                                             "SendToServer", 2);
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
       Item *, Unity::System_String *,
       Unity::il2cppArray<Unity::il2cppObject *> *)>(sendToServerPtr))(
      this, IL2CPP::String::New(func),
      Unity::il2cppArray<Unity::il2cppObject *>::Create(systemObjectClass,
                                                        args));
}
} // namespace zr