#include "TeleportWindow.hpp"
#include "IL2CPPResolver/API/Domain.hpp"
#include "IL2CPPResolver/API/Thread.hpp"
#include "IL2CPPResolver/Unity/API/Transform.hpp"
#include "IL2CPPResolver/Unity/Structures/Engine.hpp"
#include "draw/gui/window/WindowManager.hpp"
#include "imgui.h"
#include "u3d/sdk/Control/PositionControllerAC.hpp"
#include "u3d/sdk/Player/LocalPlayer.hpp"
#include "u3d/sdk/Player/Player.hpp"
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
              localplayer->teleport(position);
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
      for (const auto player : players) {
        std::string className = player->getClassName();
        PositionControllerAC *controller = player->getPositionControllerAC();
        if (controller == nullptr) {
          continue;
        }
        Unity::Vector3 position = controller->getPrevPosition();
        position.y += 1.0f;
        if (ImGui::Button(fmt::format("class :{} pos : ({}, {}, {})", className,
                                      position.x, position.y, position.z)
                              .c_str())) {
          PlayerUpdateOnceCallbackList.push_back([=](Player *player) {
            auto localplayer = LocalPlayer::getInstance();
            if (localplayer) {
              localplayer->teleport(position);
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
          localplayer->teleport(customPosition);
        }
      });
    }
    ImGui::TreePop();
  }
  IL2CPP::Thread::Detach(thread);
}
REGISTER_WINDOW(TeleportWindow);
} // namespace zr