#include "ChamsModule.hpp"
#include "IL2CPPResolver/Unity/API/Component.hpp"
#include "IL2CPPResolver/Unity/API/GameObject.hpp"
#include "IL2CPPResolver/Unity/Structures/il2cppArray.hpp"
#include "module/ModuleRegistrar.hpp"
#include "u3d/sdk/Actor/Renderer/Renderer.hpp"
#include "u3d/sdk/Actor/Renderer/Shader.hpp"
#include "u3d/sdk/PlayerUpdate.hpp"

namespace zr {
ChamsModule *ChamsModule::getInstance() {
  static ChamsModule instance;
  return &instance;
}
std::optional<std::string> ChamsModule::enable() {
  updatehandle = playerUpdateCallbacks.append([this](Player * /*player*/) {
    auto players = Player::getAllPlayers();
    for (auto player : players) {
      if (player->isInit()) {
        auto gameobject = player->GetGameObject();
        if (!gameobject)
          return;
        auto Renderer = static_cast<class Renderer *>(
            gameobject->GetComponentInChildren("UnityEngine.Renderer"));
        if (!Renderer)
          return;

        if (!Renderer->GetPropertyValue<bool>("enabled"))
          return;
        auto Material = Renderer->getMaterial();
        if (!Material)
          return;
        static Shader *chamsshader = Shader::Find("Hidden/Internal-Colored");
        if (!chamsshader)
          return;
        Material->setShader(chamsshader);
        Material->setInt("_Cull", 0);
        Material->setInt("_ZTest", 8);
        Material->setInt("_ZWrite", 1);

        Material->SetPropertyValue<Unity::Color>(
            "color", Unity::Color(Rainbow.x, Rainbow.y, Rainbow.z, Rainbow.w));
      }
    }
  });
  return std::nullopt;
}
std::optional<std::string> ChamsModule::disable() {
  playerUpdateCallbacks.remove(updatehandle);
  return std::nullopt;
}
REGISTER_MODULE(ChamsModule, ChamsModule::getInstance());

} // namespace zr