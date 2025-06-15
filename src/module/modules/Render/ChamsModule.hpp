#pragma once
#include "imgui.h"
#include "module/module.hpp"
#include "u3d/sdk/PlayerUpdate.hpp"
namespace zr {
class ChamsModule : public Module {
public:
  ChamsModule() : Module("ChamsModule", ModuleWindowType::RENDER) {}
  ImVec4 Rainbow = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
  static ChamsModule *getInstance();
  PlayerUpdateCallbackList::Handle updatehandle;
  std::optional<std::string> enable() override;
  std::optional<std::string> disable() override;
};
} // namespace zr