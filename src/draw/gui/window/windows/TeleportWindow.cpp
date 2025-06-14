#include "TeleportWindow.hpp"
#include "IL2CPPResolver/API/Domain.hpp"
#include "IL2CPPResolver/API/Thread.hpp"
#include "IL2CPPResolver/Unity/API/Transform.hpp"
#include "IL2CPPResolver/Unity/Structures/Engine.hpp"
#include "draw/gui/window/WindowManager.hpp"
#include "imgui.h"
#include "log/log.hpp"
#include "u3d/sdk/Actor/Player/LocalPlayer.hpp"
#include "u3d/sdk/Actor/Player/Player.hpp"
#include "u3d/sdk/Control/PositionControllerAC.hpp"
#include "u3d/sdk/PlayerUpdate.hpp"
#include "u3d/sdk/Rooms/Rooms.hpp"
#include <fmt/format.h>

namespace zr {
TeleportWindow *TeleportWindow::GetInstance() {
  static TeleportWindow instance;
  return &instance;
}
void TeleportWindow::OnRender() {
  auto thread = IL2CPP::Thread::Attach(IL2CPP::Domain::Get());
  if (ImGui::TreeNode("Rooms")) {
    auto rooms = Rooms::getAll();
    if (!rooms.empty()) {
      for (const auto room : rooms) {
        std::string roomName = room->getName();
        Unity::CTransform *transform = room->GetTransform();
        if (transform == nullptr) {
          continue;
        }
        Unity::Vector3 position = transform->GetPosition();
        position.y += 1.0f; // Adjust height for visibility
        if (ImGui::Button(fmt::format("Room: {} Pos: ({}, {}, {})", roomName,
                                      position.x, position.y, position.z)
                              .c_str())) {
          PlayerUpdateOnceCallbackList.push_back([=](Player *player) {
            auto localplayer = LocalPlayer::getInstance();
            if (localplayer) {
              LogInstance::getMainLogger().info(fmt::format(
                  "Teleporting to room: {} at position: ({}, {}, {})", roomName,
                  position.x, position.y, position.z));
              localplayer->teleport(position);
              localplayer->teleportClient(position);
            }
          });
        }
      }
    } else {
      ImGui::Text("No rooms available.");
    }
    ImGui::TreePop();
  }
  if (ImGui::TreeNode("Players")) {
    auto players = Player::getAllPlayers();
    if (!players.empty()) {
      int i = 0;
      for (const auto player : players) {
        std::string className = player->getClassName();
        Unity::CTransform *transform = player->GetTransform();
        if (transform == nullptr) {
          continue;
        }
        Unity::Vector3 position = transform->GetPosition();
        position.y += 1.0f;
        i += 1;
        if (ImGui::Button(
                fmt::format("class :{} index: {}", className, i).c_str())) {
          PlayerUpdateOnceCallbackList.push_back([=](Player *player) {
            auto localplayer = LocalPlayer::getInstance();
            if (localplayer) {
              LogInstance::getMainLogger().info(fmt::format(
                  "Teleporting to player: {} at position: ({}, {}, {})",
                  className, position.x, position.y, position.z));
              localplayer->teleport(position);
              localplayer->teleportClient(position);
            }
          });
        }
      }
    } else {
      ImGui::Text("No players available.");
    }
    ImGui::TreePop();
  }
  if (ImGui::TreeNode("Custom")) {
    static Unity::Vector3 customPosition;
    ImGui::InputFloat3("Custom Position", &customPosition.x);
    if (ImGui::Button("Teleport to Custom Position")) {
      PlayerUpdateOnceCallbackList.push_back([=](Player *player) {
        auto localplayer = LocalPlayer::getInstance();
        if (localplayer) {
          LogInstance::getMainLogger().info(fmt::format(
              "Teleporting to custom position: ({}, {}, {})", customPosition.x,
              customPosition.y, customPosition.z));
          localplayer->teleport(customPosition);
          localplayer->teleportClient(customPosition);
        }
      });
    }
    ImGui::TreePop();
  }
  IL2CPP::Thread::Detach(thread);
}
REGISTER_WINDOW(TeleportWindow);
} // namespace zr