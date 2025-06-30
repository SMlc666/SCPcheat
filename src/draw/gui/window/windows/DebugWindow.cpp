#include "DebugWindow.hpp"

#include "IL2CPPResolver/API/Class.hpp"

#include "IL2CPPResolver/API/Domain.hpp"
#include "IL2CPPResolver/API/String.hpp"
#include "IL2CPPResolver/API/Thread.hpp"
#include "IL2CPPResolver/Unity/API/Camera.hpp"
#include "IL2CPPResolver/Unity/API/Component.hpp"
#include "IL2CPPResolver/Unity/API/LayerMask.hpp"
#include "IL2CPPResolver/Unity/API/Object.hpp"
#include "IL2CPPResolver/Unity/API/Transform.hpp"
#include "IL2CPPResolver/Unity/Structures/Engine.hpp"
#include "IL2CPPResolver/Unity/Structures/System_String.hpp"
#include "IL2CPPResolver/Unity/Structures/il2cpp.hpp"
#include "IL2CPPResolver/Unity/Structures/il2cppArray.hpp"
#include "draw/gui/window/WindowManager.hpp"
#include "imgui.h"
#include "log/log.hpp"
#include "spdlog/fmt/bundled/format.h"
#include "u3d/sdk/Actor/Player/LocalPlayer.hpp"
#include "u3d/sdk/Actor/Player/Player.hpp"
#include "u3d/sdk/PlayerUpdate.hpp"
#include <string>
#include <vector>

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
      auto playerClass = localplayer->getPlayerClass();
      if (playerClass) {
        LogInstance::getMainLogger().info(
            "PlayerClass : {:p}", reinterpret_cast<void *>(playerClass));
        auto gameobject = playerClass->getPlayerModel();
        if (gameobject) {

          LogInstance::getMainLogger().info(
              "GameObject : {:p}", reinterpret_cast<void *>(gameobject));
          if (gameobject) {
            auto skin = gameobject->GetComponentInChildren(
                "UnityEngine.SkinnedMeshRenderer");

            if (skin) {
              LogInstance::getMainLogger().info("SkinnedMeshRenderer : {:p}",
                                                reinterpret_cast<void *>(skin));

            } else {
              LogInstance::getMainLogger().info("No SkinnedMeshRenderer found");
            }

            auto renderer =
                gameobject->GetComponentInChildren("UnityEngine.Renderer");
            if (renderer) {
              LogInstance::getMainLogger().info(
                  "Renderer : {:p}", reinterpret_cast<void *>(renderer));
            } else {
              LogInstance::getMainLogger().info("No Renderer found");
            }
          }
        } else {
          LogInstance::getMainLogger().info("No PlayerModel found");
        }
      } else {
        LogInstance::getMainLogger().info("No PlayerClass found");
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
  if (ImGui::TreeNode("All LayerMask")) {
    for (int i = 0; i < 32; i++) {
      auto layerName = Unity::LayerMask::LayerToName(i);
      if (layerName) {
        ImGui::Text("Layer %d: %s", i, layerName->ToString().c_str());
      }
    }
    ImGui::TreePop();
  }
  if (ImGui::TreeNode("Player List")) {
    auto players = Player::getAllPlayers();
    for (auto player : players) {
      if (ImGui::TreeNode(player->getAccountName().c_str())) {
        if (ImGui::TreeNode("Player Info ")) {
          ImGui::Text("Player Name: %s", player->getAccountName().c_str());
          ImGui::Text("Player Health: %d", player->getHealth());
          ImGui::Text("Player Max Health: %d", player->getMaxHealth());
          ImGui::Text("Player Class: %s", player->getClassName().c_str());
          auto playerclass = player->getPlayerClass();
          if (playerclass) {
            ImGui::Text("Player Class Name: %s",
                        playerclass->getTeamID().c_str());
          }
          auto transform = player->GetTransform();
          if (transform) {
            auto position = transform->GetPosition();

            ImGui::Text("Player Position: %.2f, %.2f, %.2f", position.x,
                        position.y, position.z);

            auto rotation = transform->GetRotation();
            ImGui::Text("Player Rotation: %.2f, %.2f, %.2f", rotation.x,
                        rotation.y, rotation.z);
          }
          ImGui::TreePop();
        }
        if (ImGui::TreeNode("components")) {
          auto gameobject = player->GetGameObject();
          if (gameobject) {
            auto components =
                gameobject->GetComponents("UnityEngine.Component");
            if (components) {
              for (int i = 0; i < components->m_uMaxLength; i++) {
                auto component = components->At(i);
                if (ImGui::TreeNode(
                        fmt::format("component {} {}", i, component->GetType())
                            .c_str())) {
                  ImGui::Text("component type: %s",
                              component->GetType().c_str());
                  ImGui::TreePop();
                }
              }
            }
          }

          ImGui::TreePop();
        }
        if (ImGui::TreeNode("model components")) {
          auto playerclass = player->getPlayerClass();
          if (playerclass) {
            auto model = playerclass->getPlayerModel();
            if (model) {
              auto components = model->GetComponents("UnityEngine.Component");
              if (components) {
                for (int i = 0; i < components->m_uMaxLength; i++) {
                  auto component = components->At(i);
                  if (ImGui::TreeNode(fmt::format("component {} {}", i,
                                                  component->GetType())
                                          .c_str())) {
                    ImGui::Text("component type: %s",
                                component->GetType().c_str());
                    ImGui::TreePop();
                  }
                }
              }
            }
          }
          ImGui::TreePop();
        }
        ImGui::TreePop();
      }
    }
    ImGui::TreePop();
  }
  if (ImGui::Button("Crasher Server")) {
    playerUpdateCallbacks.append([](Player *player) {
      auto clazz = player->getPlayerClass();
      if (clazz) {
        std::vector<Unity::il2cppObject *> args1(0);
        for (int i = 0; i < 30; i++) {
          clazz->sendToServer("Update", args1);
          clazz->sendToServer("Init", args1);
        }
      }
    });
  }
  if (ImGui::TreeNode("Class List")) {
    // 全局过滤输入框
    static char globalFilter[256] = {0};
    ImGui::InputText("Global Filter", globalFilter, sizeof(globalFilter));

    size_t m_sAssembliesCount = 0U;
    Unity::il2cppAssembly **m_pAssemblies =
        IL2CPP::Domain::GetAssemblies(&m_sAssembliesCount);

    // 收集并排序 Assemblies (DLLs)
    std::vector<Unity::il2cppAssembly *> sortedAssemblies;
    for (size_t i = 0; i < m_sAssembliesCount; ++i) {
      sortedAssemblies.push_back(m_pAssemblies[i]);
    }
    std::sort(sortedAssemblies.begin(), sortedAssemblies.end(),
              [](Unity::il2cppAssembly *a, Unity::il2cppAssembly *b) {
                return std::strcmp(a->m_pImage->m_pName, b->m_pImage->m_pName) <
                       0;
              });

    for (auto assembly : sortedAssemblies) {
      auto image = assembly->m_pImage;
      bool assemblyVisible = false;
      std::string assemblyName = image->m_pName;

      // 检查DLL是否匹配全局过滤
      bool assemblyMatches =
          globalFilter[0] == '\0' ||
          assemblyName.find(globalFilter) != std::string::npos;

      if (!assemblyMatches) {
        // 检查该DLL下的类是否匹配
        size_t m_sClassesCount =
            reinterpret_cast<size_t(IL2CPP_CALLING_CONVENTION)(void *)>(
                IL2CPP::Functions.m_ImageGetClassCount)(image);

        for (size_t j = 0; j < m_sClassesCount; ++j) {
          Unity::il2cppClass *m_pClass = reinterpret_cast<Unity::il2cppClass *(
              IL2CPP_CALLING_CONVENTION)(void *, size_t)>(
              IL2CPP::Functions.m_ImageGetClass)(image, j);

          std::string className =
              (m_pClass->m_pNamespace && m_pClass->m_pNamespace[0] != '\0')
                  ? fmt::format("{}.{}", m_pClass->m_pNamespace,
                                m_pClass->m_pName)
                  : m_pClass->m_pName;

          if (className.find(globalFilter) != std::string::npos) {
            assemblyMatches = true;
            break;
          }
        }
      }

      if (!assemblyMatches)
        continue;

      if (ImGui::TreeNode(image, "%s", image->m_pName)) {
        // DLL局部过滤
        static char dllFilter[256] = {0};
        ImGui::InputText("DLL Filter", dllFilter, sizeof(dllFilter));

        size_t m_sClassesCount =
            reinterpret_cast<size_t(IL2CPP_CALLING_CONVENTION)(void *)>(
                IL2CPP::Functions.m_ImageGetClassCount)(image);

        // 收集并排序 Classes
        std::vector<Unity::il2cppClass *> sortedClasses;
        for (size_t j = 0; j < m_sClassesCount; ++j) {
          Unity::il2cppClass *m_pClass = reinterpret_cast<Unity::il2cppClass *(
              IL2CPP_CALLING_CONVENTION)(void *, size_t)>(
              IL2CPP::Functions.m_ImageGetClass)(image, j);
          sortedClasses.push_back(m_pClass);
        }
        std::sort(sortedClasses.begin(), sortedClasses.end(),
                  [](Unity::il2cppClass *a, Unity::il2cppClass *b) {
                    std::string nameA =
                        (a->m_pNamespace && a->m_pNamespace[0] != '\0')
                            ? fmt::format("{}.{}", a->m_pNamespace, a->m_pName)
                            : a->m_pName;
                    std::string nameB =
                        (b->m_pNamespace && b->m_pNamespace[0] != '\0')
                            ? fmt::format("{}.{}", b->m_pNamespace, b->m_pName)
                            : b->m_pName;
                    return nameA < nameB;
                  });

        for (auto m_pClass : sortedClasses) {
          std::string className;
          if (m_pClass->m_pNamespace && m_pClass->m_pNamespace[0] != '\0') {
            className =
                fmt::format("{}.{}", m_pClass->m_pNamespace, m_pClass->m_pName);
          } else {
            className = m_pClass->m_pName;
          }

          // 检查类是否匹配过滤条件
          bool classMatches =
              (globalFilter[0] == '\0' ||
               className.find(globalFilter) != std::string::npos) &&
              (dllFilter[0] == '\0' ||
               className.find(dllFilter) != std::string::npos);

          if (!classMatches)
            continue;

          if (ImGui::TreeNode(m_pClass, "%s", className.c_str())) {
            // 类局部过滤
            static char classFilter[256] = {0};
            ImGui::InputText("Class Filter", classFilter, sizeof(classFilter));
            auto systemType = IL2CPP::Class::GetSystemType(m_pClass);
            if (systemType) {
              auto objectArray =
                  Unity::Object::FindObjectsOfType<Unity::CObject>(systemType);
              if (objectArray) {
                if (ImGui::TreeNode("Objects")) {
                  for (int i = 0; i < objectArray->m_uMaxLength; i++) {
                    auto obj = objectArray->At(i);
                    if (ImGui::TreeNode(obj, "%p", obj)) {
                      if (ImGui::TreeNode("Object Info")) {
                        static char objectFilter[256] = {0};
                        ImGui::InputText("Object Filter", objectFilter,
                                         sizeof(objectFilter));
                        std::vector<Unity::il2cppFieldInfo *> fields;
                        IL2CPP::Class::FetchFields(m_pClass, &fields);
                        std::sort(fields.begin(), fields.end(),
                                  [](Unity::il2cppFieldInfo *a,
                                     Unity::il2cppFieldInfo *b) {
                                    return a->m_iOffset < b->m_iOffset;
                                  });
                        for (auto field : fields) {
                          std::string showStr;
                          Unity::il2cppClass *classType =
                              IL2CPP::Class::Utils::ClassFromType(
                                  field->m_pType);
                          std::string typeString;
                          if (!std::string(classType->m_pNamespace).empty()) {
                            typeString =
                                fmt::format("{}.{}", classType->m_pNamespace,
                                            classType->m_pName);
                          } else {
                            typeString = classType->m_pName;
                          }
                          showStr = fmt::format("{}   {}", typeString,
                                                field->m_pName);
                          std::string valueString;
                          if (typeString == "System.Boolean") {
                            valueString = obj->GetMemberValue<bool>(field);
                          } else if (typeString == "System.Int32") {
                            valueString = obj->GetMemberValue<int32_t>(field);
                          } else if (typeString == "System.String") {
                            valueString =
                                obj->GetMemberValue<Unity::System_String *>(
                                       field)
                                    ->ToString();
                          } else if (typeString == "System.Single") {
                            valueString = obj->GetMemberValue<float>(field);
                          } else if (typeString == "System.Double") {
                            valueString = obj->GetMemberValue<double>(field);
                          } else {
                            valueString = "Unsupported Type";
                          }
                          showStr = fmt::format("{}   {}   0x{:016x}", showStr,
                                                valueString, field->m_iOffset);
                          ImGui::Text("%s", showStr.c_str());
                        }
                        ImGui::TreePop();
                      }
                      ImGui::TreePop();
                    }
                  }
                  ImGui::TreePop();
                }

                // 处理 Fields
                if (ImGui::TreeNode((void *)((uintptr_t)m_pClass + 0),
                                    "Fields")) {
                  static char fieldFilter[256] = {0};
                  ImGui::InputText("Field Filter", fieldFilter,
                                   sizeof(fieldFilter));

                  std::vector<Unity::il2cppFieldInfo *> fields;
                  IL2CPP::Class::FetchFields(m_pClass, &fields);

                  // 排序 Fields
                  std::sort(
                      fields.begin(), fields.end(),
                      [](Unity::il2cppFieldInfo *a, Unity::il2cppFieldInfo *b) {
                        return a->m_iOffset < b->m_iOffset;
                      });

                  for (auto field : fields) {
                    // 检查字段是否匹配过滤条件
                    bool fieldMatches =
                        fieldFilter[0] == '\0' ||
                        strstr(field->m_pName, fieldFilter) != nullptr;

                    if (!fieldMatches)
                      continue;

                    if (ImGui::TreeNode(field, "%s", field->m_pName)) {
                      ImGui::Text("offset: %d", field->m_iOffset);
                      Unity::il2cppClass *classType =
                          IL2CPP::Class::Utils::ClassFromType(field->m_pType);
                      if (classType) {
                        std::string classTypeName;
                        if (classType->m_pNamespace &&
                            classType->m_pNamespace[0] != '\0') {
                          classTypeName =
                              fmt::format("{}.{}", classType->m_pNamespace,
                                          classType->m_pName);
                        } else {
                          classTypeName = classType->m_pName;
                        }
                        ImGui::Text("class type: %s", classTypeName.c_str());
                      }
                      ImGui::TreePop();
                    }
                  }
                  ImGui::TreePop();
                }

                // 处理 Methods
                if (ImGui::TreeNode((void *)((uintptr_t)m_pClass + 1),
                                    "Methods")) {
                  static char methodFilter[256] = {0};
                  ImGui::InputText("Method Filter", methodFilter,
                                   sizeof(methodFilter));

                  std::vector<Unity::il2cppMethodInfo *> methods;
                  IL2CPP::Class::FetchMethods(m_pClass, &methods);

                  // 排序 Methods
                  std::sort(methods.begin(), methods.end(),
                            [](Unity::il2cppMethodInfo *a,
                               Unity::il2cppMethodInfo *b) {
                              return std::strcmp(a->m_pName, b->m_pName) < 0;
                            });

                  for (auto method : methods) {
                    // 检查方法是否匹配过滤条件
                    bool methodMatches =
                        methodFilter[0] == '\0' ||
                        strstr(method->m_pName, methodFilter) != nullptr;

                    if (!methodMatches)
                      continue;

                    if (ImGui::TreeNode(method, "%s", method->m_pName)) {
                      Unity::il2cppType **m_pCurrentParameterTypes =
                          method->m_pParameters;
                      int count = method->m_uArgsCount;
                      ImGui::Text("parameter count: %d", count);
                      if (count > 0) {
                        for (int i = 0; i < count; i++) {
                          Unity::il2cppType *type = m_pCurrentParameterTypes[i];
                          Unity::il2cppClass *m_pParamClass =
                              IL2CPP::Class::Utils::ClassFromType(type);
                          if (m_pParamClass) {
                            std::string paramName;
                            if (m_pParamClass->m_pNamespace &&
                                m_pParamClass->m_pNamespace[0] != '\0') {
                              paramName = fmt::format(
                                  "{}.{}", m_pParamClass->m_pNamespace,
                                  m_pParamClass->m_pName);
                            } else {
                              paramName = m_pParamClass->m_pName;
                            }
                            ImGui::Text("parameter %d: %s", i,
                                        paramName.c_str());
                          }
                        }
                      }
                      ImGui::TreePop();
                    }
                  }
                  ImGui::TreePop();
                }
              }
            }
            ImGui::TreePop();
          }
        }
        ImGui::TreePop();
      }
    }
    ImGui::TreePop();
  }
  ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
}
REGISTER_WINDOW(DebugWindow);
} // namespace zr