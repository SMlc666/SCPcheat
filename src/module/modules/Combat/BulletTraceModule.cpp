#include "BulletTraceModule.hpp"

#include "IL2CPPResolver/API/Class.hpp"
#include "IL2CPPResolver/Unity/API/Camera.hpp"
#include "IL2CPPResolver/Unity/API/Transform.hpp"
#include "IL2CPPResolver/Unity/Structures/Engine.hpp"
#include "draw/gui/gui.hpp"
#include "log/log.hpp"
#include "module/ModuleRegistrar.hpp"
#include "u3d/sdk/Actor/Player/Player.hpp"
#include "u3d/sdk/Control/FirstPersonController.hpp"
#include "u3d/sdk/sdk.hpp"

#define _USE_MATH_DEFINES
#include <cmath>
#include <limits>

namespace {
// Helper for color serialization from AimbotModule
static void toJson(nlohmann::json &j, const ImVec4 &color) {
  j = {color.x, color.y, color.z, color.w};
}

static void fromJson(const nlohmann::json &j, ImVec4 &color,
                     const ImVec4 &default_color) {
  if (j.is_array() && j.size() == 4) {
    color.x = j[0].get<float>();
    color.y = j[1].get<float>();
    color.z = j[2].get<float>();
    color.w = j[3].get<float>();
  } else {
    color = default_color;
  }
}
} // namespace

namespace zr {

// Define static members
// safetyhook::InlineHook BulletTraceModule::UpdateHook;
safetyhook::InlineHook BulletTraceModule::shootWait_MoveNextHook;
safetyhook::InlineHook BulletTraceModule::Physics_RayCastHook;
std::atomic<bool> BulletTraceModule::isInMoveNext;
bool BulletTraceModule::Physics_RayCast_Hooked(Unity::Ray *ray,
                                               Unity::RaycastHit &hitInfo,
                                               float maxDistance,
                                               int32_t layerMask) {
  // 仅当 isInMoveNext 标志为 true 时 (即在 shootWait 协程内部) 才执行我们的逻辑
  if (isInMoveNext) {
    auto *self = BulletTraceModule::getInstance();
    // 并且仅当模块启用时
    if (self->isEnabled()) {
      // --- 目标选择逻辑 (改编自您之前的代码) ---
      auto localPlayer = LocalPlayer::getInstance();
      auto mainCamera = Unity::Camera::GetMain();

      // 确保我们有进行计算所需的所有对象
      if (localPlayer && localPlayer->isInit() && mainCamera) {
        auto localtransform = localPlayer->GetTransform();
        auto localclass = localPlayer->getPlayerClass();
        if (localtransform && localclass) {
          std::string localteam = localclass->getTeamID();

          Player *bestTarget = nullptr;
          float closestValue = std::numeric_limits<float>::max();

          auto &io = ImGui::GetIO();
          Unity::Vector2 screenCenter = {io.DisplaySize.x * 0.5f,
                                         io.DisplaySize.y * 0.5f};

          auto players = Player::getAllPlayers();
          for (auto player : players) {
            if (!player || player == localPlayer || player->getHealth() <= 0 ||
                !player->isInit()) {
              continue;
            }

            auto playerTransform = player->GetTransform();
            auto playerclass = player->getPlayerClass();
            if (!playerTransform || !playerclass) {
              continue;
            }
            std::string playeam = playerclass->getTeamID();
            if (playeam.empty() || playeam == localteam ||
                playeam.find("Spectator") != std::string::npos) {
              continue;
            }
            if (self->ignoreClassD && player->getClassName() == "ClassD") {
              continue;
            }

            Unity::Vector3 playerPosition = playerTransform->GetPosition();
            Unity::Vector2 screenPosition;

            if (mainCamera->WorldToScreen(playerPosition, screenPosition)) {
              float distanceToCenter =
                  Unity::Vector2::Distance(screenPosition, screenCenter);

              if (distanceToCenter > self->aimFov) {
                continue;
              }

              float distance = Unity::Vector3::Distance(
                  localtransform->GetPosition(), playerPosition);
              if (distance > self->maxDistance) {
                continue;
              }

              if (self->aimPriority == AimPriority::Distance) {
                if (distance < closestValue) {
                  closestValue = distance;
                  bestTarget = player;
                }
              } else if (self->aimPriority == AimPriority::Health) {
                if (player->getHealth() < closestValue) {
                  closestValue = player->getHealth();
                  bestTarget = player;
                }
              } else if (self->aimPriority ==
                         AimPriority::DistanceToCrosshair) {
                if (distanceToCenter < closestValue) {
                  closestValue = distanceToCenter;
                  bestTarget = player;
                }
              }
            }
          }

          // --- 伪造 RaycastHit 的逻辑 ---
          if (bestTarget) {
            auto targetCollider = bestTarget->getCapsuleCollider();
            if (targetCollider) {
              // 填充 hitInfo 引用，让 MoveNext 以为真的打中了目标
              hitInfo.point = bestTarget->GetTransform()->GetPosition();
              hitInfo.normal =
                  (localtransform->GetPosition() - hitInfo.point).normalized();
              hitInfo.distance = Unity::Vector3::Distance(
                  localtransform->GetPosition(), hitInfo.point);
              hitInfo.m_Collider = targetCollider->GetInstanceID();
              hitInfo.faceID = 0;
              hitInfo.UV = {0, 0};

              // 返回 true，欺骗 MoveNext 函数，让它认为 Raycast 成功了
              return true;
            }
          }
        }
      }
    }
  }
  return Physics_RayCastHook.call<bool>(ray, hitInfo, maxDistance, layerMask);
}

bool BulletTraceModule::ShootWait_MoveNext_Hooked(void *unknow_instance) {
  BulletTraceModule::isInMoveNext = true;
  auto ret =
      BulletTraceModule::shootWait_MoveNextHook.call<bool>(unknow_instance);
  BulletTraceModule::isInMoveNext = false;
  return ret;
}
BulletTraceModule::BulletTraceModule()
    : Module("BulletTraceModule", ModuleWindowType::COMBAT) {}

BulletTraceModule::~BulletTraceModule() {
  // if (UpdateHook) {
  //   UpdateHook.reset();
  // }
  if (shootWait_MoveNextHook) {
    shootWait_MoveNextHook.reset();
  }
  if (Physics_RayCastHook) {
    Physics_RayCastHook.reset();
  }
}

BulletTraceModule *BulletTraceModule::getInstance() {
  static BulletTraceModule instance;
  return &instance;
}

std::optional<std::string> BulletTraceModule::load() {
  // void *weaponUpdateAddr =
  //     IL2CPP::Class::Utils::GetMethodPointer("Akequ.Base.Weapon", "Update",
  //     0);
  // if (weaponUpdateAddr == nullptr) {
  //   std::string errMsg = "BulletTraceModule: Failed to find Weapon::Update.";
  //   getLogger().error(errMsg);
  //   return errMsg;
  // }

  // UpdateHook =
  //     safetyhook::create_inline(weaponUpdateAddr, Weapon_Update_Hooked);
  // if (!UpdateHook) {
  //   std::string errMsg =
  //       "BulletTraceModule: Failed to create hook for Weapon::Update.";
  //   getLogger().error(errMsg);
  //   return errMsg;
  // }
  // getLogger().info("BulletTraceModule: Weapon::Update hooked successfully.");
  static void *MoveNextAddr = nullptr;

  if (!MoveNextAddr) {
    std::vector<Unity::il2cppClass *> m_vClasses;
    IL2CPP::Class::FetchClasses(&m_vClasses, "Assembly-CSharp", "");
    for (auto &klass : m_vClasses) {
      if (std::string(klass->m_pName) == "<shootWait>d__42") {
        MoveNextAddr =
            IL2CPP::Class::Utils::GetMethodPointer(klass, "MoveNext", 0);
        if (MoveNextAddr != nullptr) {
          break;
        } else {
          std::string errMsg =
              "BulletTraceModule: Failed to find shootWait::MoveNext.";
          getLogger().error(errMsg);
          return errMsg;
        }
      }
    }
  }
  shootWait_MoveNextHook =
      safetyhook::create_inline(MoveNextAddr, ShootWait_MoveNext_Hooked);
  if (!shootWait_MoveNextHook) {
    std::string errMsg =
        "BulletTraceModule: Failed to create hook for shootWait::MoveNext.";
    getLogger().error(errMsg);
    return errMsg;
  }
  void *RayCastAddr = IL2CPP::Class::Utils::GetMethodPointer(
      "UnityEngine.Physics", "Raycast",
      {"UnityEngine.Ray", "UnityEngine.RaycastHit", "System.Single",
       "System.Int32"});
  if (RayCastAddr == nullptr) {
    std::string errMsg = "BulletTraceModule: Failed to find Physics::Raycast.";
    getLogger().error(errMsg);
    return errMsg;
  }
  Physics_RayCastHook =
      safetyhook::create_inline(RayCastAddr, Physics_RayCast_Hooked);
  if (!Physics_RayCastHook) {
    std::string errMsg =
        "BulletTraceModule: Failed to create hook for Physics::Raycast.";
    getLogger().error(errMsg);
    return errMsg;
  }
  return std::nullopt;
}

std::optional<std::string> BulletTraceModule::enable() {
  getLogger().info("BulletTraceModule enabled.");
  renderHandle = zr::ImGuiCallback.append([this]() { this->renderFov(); });
  return std::nullopt;
}

std::optional<std::string> BulletTraceModule::disable() {
  getLogger().info("BulletTraceModule disabled.");
  zr::ImGuiCallback.remove(renderHandle);
  return std::nullopt;
}

// void BulletTraceModule::Weapon_Update_Hooked(Weapon *m_pWeapon) {
//   auto *self = BulletTraceModule::getInstance();
//   if (!self->isEnabled()) {
//     UpdateHook.call(m_pWeapon);
//     return;
//   }

//   auto localPlayer = LocalPlayer::getInstance();
//   if (!localPlayer || !localPlayer->isInit()) {
//     UpdateHook.call(m_pWeapon);
//     return;
//   }
//   auto localtransform = localPlayer->GetTransform();
//   if (!localtransform) {
//     UpdateHook.call(m_pWeapon);
//     return;
//   }
//   auto mainCamera = Unity::Camera::GetMain();
//   if (!mainCamera) {
//     UpdateHook.call(m_pWeapon);
//     return;
//   }
//   auto localclass = localPlayer->getPlayerClass();
//   if (!localclass) {
//     UpdateHook.call(m_pWeapon);
//     return;
//   }
//   std::string localteam = localclass->getTeamID();
//   if (localteam.empty()) {
//     UpdateHook.call(m_pWeapon);
//     return;
//   }

//   auto players = Player::getAllPlayers();
//   Player *bestTarget = nullptr;
//   float closestValue = std::numeric_limits<float>::max();

//   auto &io = ImGui::GetIO();
//   Unity::Vector2 screenCenter = {io.DisplaySize.x * 0.5f,
//                                  io.DisplaySize.y * 0.5f};

//   for (auto player : players) {
//     if (!player || player == localPlayer || player->getHealth() <= 0 ||
//         !player->isInit()) {
//       continue;
//     }

//     auto playerTransform = player->GetTransform();
//     if (!playerTransform) {
//       continue;
//     }
//     auto playerclass = player->getPlayerClass();
//     if (!playerclass) {
//       continue;
//     }
//     std::string playeam = playerclass->getTeamID();
//     if (playeam.empty() || playeam == localteam ||
//         playeam.find("Spectator") != std::string::npos) {
//       continue;
//     }
//     if (self->ignoreClassD && player->getClassName() == "ClassD") {
//       continue;
//     }

//     Unity::Vector3 playerPosition = playerTransform->GetPosition();
//     Unity::Vector2 screenPosition;

//     if (mainCamera->WorldToScreen(playerPosition, screenPosition)) {
//       float distanceToCenter =
//           Unity::Vector2::Distance(screenPosition, screenCenter);

//       if (distanceToCenter > self->aimFov) {
//         continue;
//       }

//       float distance = Unity::Vector3::Distance(
//           localPlayer->GetTransform()->GetPosition(), playerPosition);
//       if (distance > self->maxDistance) {
//         continue;
//       }

//       if (self->aimPriority == AimPriority::Distance) {
//         if (distance < closestValue) {
//           closestValue = distance;
//           bestTarget = player;
//         }
//       } else if (self->aimPriority == AimPriority::Health) {
//         if (player->getHealth() < closestValue) {
//           closestValue = player->getHealth();
//           bestTarget = player;
//         }
//       } else if (self->aimPriority == AimPriority::DistanceToCrosshair) {
//         if (distanceToCenter < closestValue) {
//           closestValue = distanceToCenter;
//           bestTarget = player;
//         }
//       }
//     }
//   }

//   if (bestTarget) {
//     auto fpc = localPlayer->getFirstPersonController();
//     if (fpc) {
//       auto mouseLook = fpc->getMouseLook();
//       if (mouseLook) {
//         // Save original rotation
//         Unity::Quaternion originalCharacterRot =
//             mouseLook->getCharacterTargetRot();
//         Unity::Quaternion originalCameraRot =
//         mouseLook->getCameraTargetRot();

//         // Calculate and set new rotation
//         auto targetTransform = bestTarget->GetTransform();
//         Unity::Vector3 direction =
//             (targetTransform->GetPosition() - localtransform->GetPosition())
//                 .normalized();

//         float yaw = atan2f(direction.x, direction.z) * 180.0f / M_PI;
//         float pitch = -asinf(direction.y) * 180.0f / M_PI;

//         Unity::Quaternion characterRotation =
//             Unity::Quaternion::FromEuler(0.0f, yaw, 0.0f);
//         Unity::Quaternion cameraRotation =
//             Unity::Quaternion::FromEuler(pitch, 0.0f, 0.0f);

//         mouseLook->setCharacterTargetRot(characterRotation);
//         mouseLook->setCameraTargetRot(cameraRotation);

//         // Shoot with new rotation
//         UpdateHook.call(m_pWeapon);

//         // Restore original rotation
//         mouseLook->setCharacterTargetRot(originalCharacterRot);
//         mouseLook->setCameraTargetRot(originalCameraRot);
//         return; // Early return to avoid double call
//       }
//     }
//   }

//   // If no target or components found, just call original
//   UpdateHook.call(m_pWeapon);
// }

std::optional<std::string> BulletTraceModule::drawGUI() {
  ImGui::Text("Bullet Trace Settings");
  ImGui::Separator();

  ImGui::SliderFloat("FOV", &aimFov, 1.0f, 360.0f, "%.1f");
  ImGui::SliderFloat("Max Distance", &maxDistance, 1.0f, 1000.0f, "%.1f m");
  ImGui::Checkbox("Ignore Class D", &ignoreClassD);
  ImGui::Separator();
  ImGui::Checkbox("Draw FOV", &drawFov);
  ImGui::ColorEdit4("FOV Color", &fovColor.x);

  const char *priorityItems[] = {"Distance", "Health", "Distance to Crosshair"};
  int currentPriority = static_cast<int>(aimPriority);
  if (ImGui::Combo("Aim Priority", &currentPriority, priorityItems,
                   IM_ARRAYSIZE(priorityItems))) {
    aimPriority = static_cast<AimPriority>(currentPriority);
  }
  return std::nullopt;
}

std::optional<std::string>
BulletTraceModule::toJson(nlohmann::json &json) const {
  json["aimFov"] = aimFov;
  json["maxDistance"] = maxDistance;
  json["ignoreClassD"] = ignoreClassD;
  json["aimPriority"] = static_cast<int>(aimPriority);
  json["drawFov"] = drawFov;
  ::toJson(json["fovColor"], fovColor);
  return std::nullopt;
}

std::optional<std::string>
BulletTraceModule::fromJson(const nlohmann::json &json) {
  if (json.contains("aimFov"))
    aimFov = json["aimFov"].get<float>();
  if (json.contains("maxDistance"))
    maxDistance = json["maxDistance"].get<float>();
  if (json.contains("ignoreClassD"))
    ignoreClassD = json["ignoreClassD"].get<bool>();
  if (json.contains("aimPriority"))
    aimPriority = static_cast<AimPriority>(json["aimPriority"].get<int>());
  if (json.contains("drawFov"))
    drawFov = json["drawFov"].get<bool>();
  if (json.contains("fovColor"))
    ::fromJson(json["fovColor"], fovColor, {1.0f, 1.0f, 1.0f, 0.3f});
  return std::nullopt;
}

void BulletTraceModule::renderFov() {
  auto drawList = ImGui::GetBackgroundDrawList();
  auto &io = ImGui::GetIO();
  drawList->AddCircle(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f),
                      aimFov, ImGui::ColorConvertFloat4ToU32(fovColor), 64,
                      1.0f);
}
// REGISTER_MODULE(BulletTraceModule, BulletTraceModule::getInstance());

} // namespace zr
