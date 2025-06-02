#include "ModuleWindow.hpp"
#include "draw/gui/window/WindowManager.hpp"
#include "draw/gui/window/windows/ModuleWindowType.hpp"
#include "imgui.h"
#include "module/ModuleManager.hpp"

zr::ModuleWindow *zr::ModuleWindow::GetInstance() {
  static ModuleWindow instance;
  return &instance;
}

void zr::ModuleWindow::OnRender() {
  if (ImGui::BeginTabBar("ModuleWindowTabBar")) {
    for (auto &moduleTypeString : module_window_type_to_string) {
      if (moduleTypeString.first != ModuleWindowType::NONE &&
          ImGui::BeginTabItem(moduleTypeString.second.c_str())) {
        ModuleManager::getInstance().forEachModule([=](Module *module) {
          if (module->getWindowType() == moduleTypeString.first) {
            if (ImGui::TreeNode(module->getName().c_str())) {
              bool moduleEnable = module->isEnabled();
              if (ImGui::Checkbox("Enabled", &moduleEnable)) {
                if (moduleEnable && !module->isEnabled()) {
                  module->enableModule();
                } else if (!moduleEnable && module->isEnabled()) {
                  module->disableModule();
                }
              }
              module->drawGUIModule();
              ImGui::TreePop();
            }
          }
          return true;
        });
        ImGui::EndTabItem();
      }
    }
    ImGui::EndTabBar();
  }
}

namespace zr {
REGISTER_WINDOW(ModuleWindow);
}