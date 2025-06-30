#pragma once

#include "IL2CPPResolver/Unity/API/Camera.hpp"
#include "IL2CPPResolver/Unity/Structures/il2cppArray.hpp"
#include "IL2CPPResolver/Unity/Structures/System_String.hpp"
#include "IL2CPPResolver/Unity/Structures/Vector3.hpp"
#include "draw/gui/gui.hpp"
#include "imgui.h"
#include "module/module.hpp"
#include "safetyhook.hpp"
#include "u3d/sdk/Actor/Player/LocalPlayer.hpp"
#include "u3d/sdk/Actor/Player/Player.hpp"
#include "u3d/sdk/Actor/Player/PlayerClass.hpp"
#include <optional>
#include <string>
#include <vector>

namespace zr {

class SCPModule : public Module {
private:
  bool autoAttackEnabled = false;
  float attackRange = 10.0f; // 默认攻击范围10米
  int attackInterval = 10;   // 默认攻击间隔10帧
  int autoAttackCount = 1;   // 默认自动攻击次数1次
  bool superAttackEnabled = false;
  int superAttackCount = 1; // 默认超级攻击次数1次
  int frameCounter = 0;

  safetyhook::InlineHook playerClassUpdateHook;
  safetyhook::InlineHook playerClassSendToServerHook;

  // 代理函数声明
  static void PlayerClass_Update_Proxy(PlayerClass *this_ptr);
  static void PlayerClass_SendToServer_Proxy(
      PlayerClass *this_ptr, Unity::System_String *func,
      Unity::il2cppArray<Unity::il2cppObject *> *args);

  // 模块内部逻辑函数
  void onPlayerClassUpdate(PlayerClass *this_ptr);
  void onPlayerClassSendToServer(PlayerClass *this_ptr,
                                 Unity::System_String *func,
                                 Unity::il2cppArray<Unity::il2cppObject *> *args);

public:
  SCPModule();
  static SCPModule *getInstance();

  std::optional<std::string> enable() override;
  std::optional<std::string> disable() override;
  std::optional<std::string> drawGUI() override;
  std::optional<std::string> toJson(nlohmann::json &json) const override;
  std::optional<std::string> fromJson(const nlohmann::json &json) override;
};

} // namespace zr