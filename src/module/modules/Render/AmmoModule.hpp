#pragma once

#include "module/module.hpp"
#include "u3d/sdk/Base/AmmoData.hpp"
#include "u3d/sdk/PlayerUpdate.hpp"
#include "imgui.h"
#include <mutex>
#include <vector>

namespace zr {

class AmmoModule : public Module {
private:
    struct AmmoDisplayData {
        std::string ammoType;
        uint16_t current;
        uint16_t maxAmmo;
        std::string ui;
    };

    std::vector<AmmoDisplayData> ammoDataCopy;
    std::mutex dataMutex;
    PlayerUpdateCallbackList::Handle playerUpdateHandle;
    eventpp::CallbackList<void()>::Handle imGuiCallbackHandle;

    // 配置选项
    ImVec2 position = {20.0f, 100.0f};
    float fontSize = 14.0f;
    ImVec4 textColor = {1.0f, 1.0f, 1.0f, 1.0f};
    bool showAmmoType = true;
    bool showCurrent = true;
    bool showMaxAmmo = true;
    bool showUI = true;
    float verticalSpacing = 5.0f;
    bool showBackground = false;
    ImVec4 backgroundColor = {0.2f, 0.2f, 0.2f, 0.7f};
    float backgroundPadding = 5.0f;
    bool autoPosition = true;

    void onPlayerUpdate(Player* player);
    void onImGuiRender();

public:
    AmmoModule();
    static AmmoModule* getInstance();

    std::optional<std::string> enable() override;
    std::optional<std::string> disable() override;
    std::optional<std::string> drawGUI() override;
    std::optional<std::string> toJson(nlohmann::json& json) const override;
    std::optional<std::string> fromJson(const nlohmann::json& json) override;
};

} // namespace zr
