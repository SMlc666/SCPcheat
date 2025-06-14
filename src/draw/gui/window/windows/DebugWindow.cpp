#include "DebugWindow.hpp"

#include "IL2CPPResolver/API/Class.hpp"

#include "IL2CPPResolver/API/Domain.hpp"
#include "IL2CPPResolver/API/Thread.hpp"
#include "IL2CPPResolver/Unity/API/Camera.hpp"
#include "IL2CPPResolver/Unity/API/Component.hpp"
#include "IL2CPPResolver/Unity/API/Transform.hpp"
#include "IL2CPPResolver/Unity/Structures/Engine.hpp"
#include "draw/gui/window/WindowManager.hpp"
#include "imgui.h"
#include "log/log.hpp"
#include "u3d/sdk/Actor/Player/LocalPlayer.hpp"

namespace zr {
DebugWindow *DebugWindow::GetInstance() {
  static DebugWindow instance;
  return &instance;
}

void DebugWindow::OnRender() {
  if (ImGui::Button("Get Player::Update() pointer")) {
    void *PlayerUpdatePtr =
        IL2CPP::Class::Utils::GetMethodPointer("Player", "Update", 0);
    LogInstance::getMainLogger().info(
        "Player::Update() pointer: {:p}",
        reinterpret_cast<void *>(PlayerUpdatePtr));
  }
  if (ImGui::Button("Get LocalPlayer")) {
    LocalPlayer *localplayer = LocalPlayer::getInstance();
    LogInstance::getMainLogger().info("LocalPlayer {:p}",
                                      reinterpret_cast<void *>(localplayer));
    if (localplayer) {
      void *thisThread = IL2CPP::Thread::Attach(IL2CPP::Domain::Get());
      Unity::CTransform *transform = localplayer->GetTransform();
      LogInstance::getMainLogger().info("Transform : {:p}",
                                        reinterpret_cast<void *>(transform));
      LogInstance::getMainLogger().info(
          "name : {}", localplayer->getPlayerClass()->getName());

      Unity::Vector3 vec3 = transform->GetPosition();
      LogInstance::getMainLogger().info(" x y z:", vec3.x, vec3.y, vec3.z);
      auto gameobject = localplayer->GetGameObject();
      LogInstance::getMainLogger().info("GameObject : {:p}",
                                        reinterpret_cast<void *>(gameobject));
      if (gameobject) {
        auto skin = gameobject->GetComponentInChildren(
            "UnityEngine.SkinnedMeshRenderer");
        if (skin) {
          LogInstance::getMainLogger().info("SkinnedMeshRenderer : {:p}",
                                            reinterpret_cast<void *>(skin));

        } else {
          LogInstance::getMainLogger().info("No SkinnedMeshRenderer found");
        }
      }
      IL2CPP::Thread::Detach(thisThread);
    }
  }
  if (ImGui::Button("Get get_transform")) {
    void *get_transformPtr = IL2CPP::Class::Utils::GetMethodPointer(
        "UnityEngine.Component", "get_transform", 0);
    LogInstance::getMainLogger().info("transformPtr : {:p}", get_transformPtr);
  }
  if (ImGui::Button("get Main Camera")) {

    LogInstance::getMainLogger().info(
        "Main Camera pointer: {:p}",
        reinterpret_cast<void *>(Unity::Camera::GetMain()));
  }
  if (ImGui::Button("Get Current Camera")) {
    LogInstance::getMainLogger().info(
        "Current Camera pointer: {:p}",
        reinterpret_cast<void *>(Unity::Camera::GetCurrent()));
  }
  if (ImGui::Button("Try Crash")) {
    // This will cause a crash, triggering the crash dump handler
    int *p = nullptr;
    *p = 42; // Dereferencing a null pointer
  }
  ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
}
REGISTER_WINDOW(DebugWindow);
} // namespace zr