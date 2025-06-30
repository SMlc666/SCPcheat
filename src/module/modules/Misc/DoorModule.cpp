#include "DoorModule.hpp"
#include "IL2CPPResolver/Unity/API/Component.hpp"
#include "IL2CPPResolver/Unity/API/Object.hpp"
#include "IL2CPPResolver/Unity/API/Transform.hpp"
#include "imgui.h"
#include "module/ModuleRegistrar.hpp"
#include "u3d/sdk/Actor/Player/Player.hpp"
#include "u3d/sdk/PlayerUpdate.hpp"
#include "u3d/sdk/Rooms/Door.hpp"
#include <mutex>

namespace zr {
std::optional<std::string> DoorModule::disable() {
  playerUpdateCallbacks.remove(updatehandle);
  return std::nullopt;
}
std::optional<std::string> DoorModule::enable() {
  updatehandle = playerUpdateCallbacks.append([this](Player *player) {
    auto objects = Unity::Object::FindObjectsOfType<Door>("Door");
    if (objects) {
      for (int i = 0; i < objects->m_uMaxLength; i++) {
        Door *door = objects->At(i);
        if (door) {
          if (always_open && !door->getOpened()) {
            door->forceState(true);
          }
          Unity::CTransform *transform = door->GetTransform();
          if (transform) {
            door->breakDoor(transform->GetPosition(), true);
            if (usingClientBreak) {
              door->breakDoorClient(transform->GetPosition());
            }
          } else {
            static std::once_flag once;
            std::call_once(once, [this]() {
              getLogger().error("Failed to get transform of door");
            });
          }

        } else {
          static std::once_flag once;
          std::call_once(once, [this]() {
            getLogger().error("Failed to find Door objects");
          });
        }
      }
    }
    return std::nullopt;
  });
  return std::nullopt;
}

std::optional<std::string> DoorModule::fromJson(const nlohmann::json &json) {
  always_break = json.value("always_break", false);
  always_open = json.value("always_open", false);
  usingClientBreak = json.value("usingClientBreak", false);
  return std::nullopt;
}
std::optional<std::string> DoorModule::toJson(nlohmann::json &json) const {
  json["always_open"] = always_open;
  json["always_break"] = always_break;
  json["usingClientBreak"] = usingClientBreak;
  return std::nullopt;
}
std::optional<std::string> DoorModule::load() { return std::nullopt; }
std::optional<std::string> DoorModule::drawGUI() {
  ImGui::Checkbox("always_open", &always_open);
  ImGui::Checkbox("always_break", &always_break);
  ImGui::Checkbox("usingClientBreak", &usingClientBreak);
  return std::nullopt;
}
DoorModule *DoorModule::getInstance() {
  static DoorModule instance;
  return &instance;
}
REGISTER_MODULE(DoorModule, DoorModule::getInstance());
} // namespace zr