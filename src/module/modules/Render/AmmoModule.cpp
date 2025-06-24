#include "AmmoModule.hpp"
#include "u3d/sdk/Actor/Player/Player.hpp"
#include "module/ModuleRegistrar.hpp"
#include "log/log.hpp"
#include <fmt/format.h>
#include "draw/gui/gui.hpp"
namespace zr {

AmmoModule::AmmoModule() 
    : Module("AmmoModule", ModuleWindowType::RENDER) {
    getLogger().info("AmmoModule initialized");
}

AmmoModule* AmmoModule::getInstance() {
    static AmmoModule instance;
    return &instance;
}

std::optional<std::string> AmmoModule::enable() {
    playerUpdateHandle = playerUpdateCallbacks.append(
        [this](Player* player) { this->onPlayerUpdate(player); });
    
    imGuiCallbackHandle = ImGuiCallback.append(
        [this]() { this->onImGuiRender(); });

    getLogger().info("AmmoModule enabled");
    return std::nullopt;
}

std::optional<std::string> AmmoModule::disable() {
    playerUpdateCallbacks.remove(playerUpdateHandle);
    ImGuiCallback.remove(imGuiCallbackHandle);
    
    getLogger().info("AmmoModule disabled");
    return std::nullopt;
}

void AmmoModule::onPlayerUpdate(Player* player) {
    if (!player || !player->isInit()) return;

    std::lock_guard<std::mutex> lock(dataMutex);
    auto ammoData = player->getAmmoData();
    ammoDataCopy.clear();

    for (auto ammo : ammoData) {
        if (!ammo) continue;
        ammoDataCopy.push_back({
            ammo->getAmmoType(),
            ammo->getCurrent(),
            ammo->getMaxAmmo(),
            ammo->getUI()
        });
    }
}

void AmmoModule::onImGuiRender() {
    if (!isEnabled()) return;

    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
    ImGui::SetNextWindowSizeConstraints(ImVec2(200, 100), ImVec2(400, 800));

    if (autoPosition) {
        ImVec2 viewportSize = ImGui::GetMainViewport()->Size;
        position = ImVec2(20.0f, viewportSize.y * 0.5f);
    }

    ImGui::SetNextWindowPos(position);

    if (showBackground) {
        ImGui::PushStyleColor(ImGuiCol_WindowBg, backgroundColor);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, 
                           ImVec2(backgroundPadding, backgroundPadding));
    }

    ImGui::Begin("Ammo Display", nullptr, 
                ImGuiWindowFlags_NoTitleBar | 
                ImGuiWindowFlags_NoResize | 
                ImGuiWindowFlags_NoMove);

    std::lock_guard<std::mutex> lock(dataMutex);
    for (const auto& ammo : ammoDataCopy) {
        std::string displayText;
        if (showAmmoType) displayText += ammo.ammoType + ": ";
        if (showCurrent) displayText += std::to_string(ammo.current);
        if (showMaxAmmo) displayText += "/" + std::to_string(ammo.maxAmmo);
        if (showUI && !ammo.ui.empty()) displayText += " (" + ammo.ui + ")";

        ImGui::TextColored(textColor, "%s", displayText.c_str());
        ImGui::Dummy(ImVec2(0, verticalSpacing));
    }

    ImGui::End();

    if (showBackground) {
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
    }
    ImGui::PopFont();
}

std::optional<std::string> AmmoModule::drawGUI() {
    ImGui::Checkbox("Auto Position", &autoPosition);
    if (!autoPosition) {
        ImGui::DragFloat2("Position", &position.x, 1.0f);
    }
    
    ImGui::DragFloat("Font Size", &fontSize, 0.5f, 8.0f, 32.0f);
    ImGui::ColorEdit4("Text Color", &textColor.x);
    ImGui::DragFloat("Line Spacing", &verticalSpacing, 0.5f, 0.0f, 20.0f);
    
    ImGui::Separator();
    ImGui::Checkbox("Show Background", &showBackground);
    if (showBackground) {
        ImGui::ColorEdit4("Background Color", &backgroundColor.x);
        ImGui::DragFloat("Background Padding", &backgroundPadding, 0.5f, 0.0f, 20.0f);
    }
    
    ImGui::Separator();
    ImGui::Text("Display Options:");
    ImGui::Checkbox("Show Ammo Type", &showAmmoType);
    ImGui::Checkbox("Show Current Ammo", &showCurrent);
    ImGui::Checkbox("Show Max Ammo", &showMaxAmmo);
    ImGui::Checkbox("Show UI Info", &showUI);

    return std::nullopt;
}

std::optional<std::string> AmmoModule::toJson(nlohmann::json& json) const {
    json["position"] = {position.x, position.y};
    json["fontSize"] = fontSize;
    json["textColor"] = {textColor.x, textColor.y, textColor.z, textColor.w};
    json["verticalSpacing"] = verticalSpacing;
    json["autoPosition"] = autoPosition;
    json["showBackground"] = showBackground;
    json["backgroundColor"] = {backgroundColor.x, backgroundColor.y, 
                              backgroundColor.z, backgroundColor.w};
    json["backgroundPadding"] = backgroundPadding;
    json["showAmmoType"] = showAmmoType;
    json["showCurrent"] = showCurrent;
    json["showMaxAmmo"] = showMaxAmmo;
    json["showUI"] = showUI;
    return std::nullopt;
}

std::optional<std::string> AmmoModule::fromJson(const nlohmann::json& json) {
    if (json.contains("position")) {
        position.x = json["position"][0].get<float>();
        position.y = json["position"][1].get<float>();
    }
    fontSize = json.value("fontSize", 14.0f);
    if (json.contains("textColor")) {
        textColor.x = json["textColor"][0].get<float>();
        textColor.y = json["textColor"][1].get<float>();
        textColor.z = json["textColor"][2].get<float>();
        textColor.w = json["textColor"][3].get<float>();
    }
    verticalSpacing = json.value("verticalSpacing", 5.0f);
    autoPosition = json.value("autoPosition", true);
    showBackground = json.value("showBackground", false);
    if (json.contains("backgroundColor")) {
        backgroundColor.x = json["backgroundColor"][0].get<float>();
        backgroundColor.y = json["backgroundColor"][1].get<float>();
        backgroundColor.z = json["backgroundColor"][2].get<float>();
        backgroundColor.w = json["backgroundColor"][3].get<float>();
    }
    backgroundPadding = json.value("backgroundPadding", 5.0f);
    showAmmoType = json.value("showAmmoType", true);
    showCurrent = json.value("showCurrent", true);
    showMaxAmmo = json.value("showMaxAmmo", true);
    showUI = json.value("showUI", true);
    return std::nullopt;
}

REGISTER_MODULE(AmmoModule, AmmoModule::getInstance())

} // namespace zr