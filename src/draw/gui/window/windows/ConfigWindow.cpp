#include "ConfigWindow.hpp"
#include "config/config.hpp"
#include "draw/gui/window/WindowManager.hpp"
#include "imgui.h"
#include "log/log.hpp"
#include <fstream>
#include <sstream>

namespace zr {

ConfigWindow *ConfigWindow::GetInstance() {
  static ConfigWindow instance;
  return &instance;
}

ConfigWindow::ConfigWindow() : Window("Config", true) {}

void ConfigWindow::OnRender() {
  static std::filesystem::path configFilePath =
      ConfigHelper::getConfigFilePath();
  static std::string configFileContent;

  if (ImGui::Button("Load Config")) {
    ConfigHelper::loadConfigFromFile(configFilePath);
    // 读取文件内容
    std::ifstream ifs(configFilePath);
    if (ifs.is_open()) {
      std::stringstream buffer;
      buffer << ifs.rdbuf();
      configFileContent = buffer.str();
      ifs.close();
      LogInstance::getMainLogger().info("Config loaded from {}",
                                        configFilePath.string());
    } else {
      LogInstance::getMainLogger().error("Failed to open config file: {}",
                                         configFilePath.string());
      configFileContent = "";
    }
  }

  ImGui::SameLine();

  if (ImGui::Button("Save Config")) {
    ConfigHelper::saveConfigToFile(configFilePath);
    LogInstance::getMainLogger().info("Config saved to {}",
                                      configFilePath.string());
  }

  ImGui::Text("Config file path: %s", configFilePath.string().c_str());

  ImGui::Separator();

  ImGui::Text("Config File Content (Read-Only):");
  ImGui::BeginChild("ConfigContent", ImVec2(0, 300), true,
                    ImGuiWindowFlags_HorizontalScrollbar);
  ImGui::TextUnformatted(configFileContent.c_str());
  ImGui::EndChild();
}

REGISTER_WINDOW(ConfigWindow);

} // namespace zr