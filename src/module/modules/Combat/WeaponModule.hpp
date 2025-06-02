#pragma once
#include "draw/gui/window/windows/ModuleWindowType.hpp"
#include "module/module.hpp"
#include "nlohmann/json_fwd.hpp"
#include "u3d/sdk/PlayerUpdate.hpp"

namespace zr {

class WeaponModule : public Module {
private:
  WeaponModule() : Module("WeaponModule", ModuleWindowType::COMBAT) {}

public:
  static WeaponModule *getInstance();
  float shootSpeed = 0.1;
  float recoil = 0;
  float spread = 0;
  float size_comp = 1;
  // 模块生命周期方法
  std::optional<std::string> load() override;
  std::optional<std::string> enable() override;
  std::optional<std::string> disable() override;
  std::optional<std::string> drawGUI() override;
  std::optional<std::string> toJson(nlohmann::json &json) const override;
  std::optional<std::string> fromJson(const nlohmann::json &json) override;
};

} // namespace zr