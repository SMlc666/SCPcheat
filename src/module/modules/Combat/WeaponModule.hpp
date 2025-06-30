#pragma once
#include "draw/gui/window/windows/ModuleWindowType.hpp"
#include "module/module.hpp"
#include "nlohmann/json_fwd.hpp"
#include "u3d/sdk/PlayerUpdate.hpp"

namespace zr {

// 新增：定义攻击模式
enum class AutoAttackMode {
  Single, // 单体
  Multi   // 群体
};

// 新增：定义攻击优先级
enum class AutoAttackPriority {
  Distance, // 距离
  Health    // 血量
};

class WeaponModule : public Module {
private:
  WeaponModule() : Module("WeaponModule", ModuleWindowType::COMBAT) {}

public:
  int frameCounter = 0; // 新增：用于计算攻击间隔
  static WeaponModule *getInstance();
  bool changeShootSpeed = false;
  float shootSpeed = 0.1;
  float recoil = 0;
  float spread = 0;
  float size_comp = 1;
  float reloadingTime = 0.1;
  bool isReloading = false;
  bool anticheat = false;
  bool friendlyFire = true;
  bool infiniteAmmo = false;
  bool autoAttack = false;
  int attackCount = 1;
  bool spoofYourAttack = false;
  bool superBullet = false;
  int bulletCount = 1;

  // --- 新增 AutoAttack 相关设置 ---
  float attackRange = 15.0f;
  int attackInterval = 10;
  AutoAttackMode attackMode = AutoAttackMode::Multi;
  AutoAttackPriority attackPriority = AutoAttackPriority::Distance;
  bool ignoreClassD = true;
  bool ignoreSCP999 = true;
   bool raycast = false;
  float attackFailChance = 0.0f;
  // ---------------------------------
  // 模块生命周期方法
  std::optional<std::string> load() override;
  std::optional<std::string> enable() override;
  std::optional<std::string> disable() override;
  std::optional<std::string> drawGUI() override;
  std::optional<std::string> toJson(nlohmann::json &json) const override;
  std::optional<std::string> fromJson(const nlohmann::json &json) override;
};

} // namespace zr