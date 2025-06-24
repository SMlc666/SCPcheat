#include "BypassModule.hpp"
#include "IL2CPPResolver/API/Class.hpp"
#include "IL2CPPResolver/API/String.hpp"
#include "IL2CPPResolver/Unity/API/Component.hpp"
#include "IL2CPPResolver/Unity/API/Object.hpp"
#include "IL2CPPResolver/Unity/Structures/System_String.hpp"
#include "IL2CPPResolver/Unity/Structures/il2cppArray.hpp"
#include "IL2CPPResolver/Unity/Structures/il2cppDictionary.hpp"
#include "imgui.h"
#include "module/ModuleRegistrar.hpp"
#include "module/modules/Misc/BypassModule.hpp"
#include "safetyhook/easy.hpp"
#include "safetyhook/inline_hook.hpp"
#include <Windows.h>
#include <random>
namespace {
safetyhook::InlineHook IsDebuggerPresent_hook;
safetyhook::InlineHook PluginAuthenicator_GetHash_hook;

bool isDebuggerPresent_Proxy() {
  if (zr::BypassModule::getInstance()->isEnabled()) {
    zr::BypassModule::getInstance()->getLogger().info(
        "Bypass Debuger Status: {}", zr::BypassModule::getInstance()->isDebug);
    return zr::BypassModule::getInstance()->isDebug;
  }
  return IsDebuggerPresent_hook.call<bool>();
}
Unity::System_String *PluginAuthenicator_GetHash(Unity::System_String *str) {
  if (zr::BypassModule::getInstance()->isEnabled() &&
      zr::BypassModule::getInstance()->randomDeviceID) {
    // 随机生成64位字符串
    const std::string charset =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, charset.size() - 1);

    // 生成64位随机字符串
    std::string randomStr;
    for (int i = 0; i < 64; ++i) {
      randomStr += charset[dis(gen)];
    }

    // 将 std::string 转换为 Unity::System_String *
    return IL2CPP::String::New(randomStr.c_str());
  }
  return PluginAuthenicator_GetHash_hook.call<Unity::System_String *>(str);
}
} // namespace
namespace zr {
BypassModule *BypassModule::getInstance() {
  static BypassModule instance;
  return &instance;
}

std::optional<std::string> BypassModule::enable() { return std::nullopt; }
std::optional<std::string> BypassModule::disable() { return std::nullopt; }
std::optional<std::string> BypassModule::load() {
  HMODULE kernel32 = GetModuleHandleA("kernel32.dll");
  if (kernel32 == nullptr) {
    return "Failed to get kernel32.dll";
  }
  FARPROC IsDebuggerPresent_ptr = GetProcAddress(kernel32, "IsDebuggerPresent");
  if (IsDebuggerPresent_ptr == nullptr) {
    return "Failed to get IsDebuggerPresent";
  } else {
    getLogger().info("IsDebuggerPresent: {:p}", (void *)IsDebuggerPresent_ptr);
  }
  IsDebuggerPresent_hook =
      safetyhook::create_inline(IsDebuggerPresent_ptr, isDebuggerPresent_Proxy);
  void *PluginAuthenicator_GetHash_ptr = IL2CPP::Class::Utils::GetMethodPointer(
      "PluginAuthenicator", "GetHash", 1);
  if (PluginAuthenicator_GetHash_ptr == nullptr) {
    return "Failed to get PluginAuthenicator_GetHash";
  } else {
    getLogger().info("PluginAuthenicator_GetHash: {:p}",
                     (void *)PluginAuthenicator_GetHash_ptr);
  }
  PluginAuthenicator_GetHash_hook = safetyhook::create_inline(
      PluginAuthenicator_GetHash_ptr, PluginAuthenicator_GetHash);
  return std::nullopt;
}

std::optional<std::string> BypassModule::toJson(nlohmann::json &j) const {
  j["isDebug"] = isDebug;
  j["randomDeviceID"] = randomDeviceID;

  return std::nullopt;
}
std::optional<std::string> BypassModule::fromJson(const nlohmann::json &j) {
  if (j.contains("isDebug") && j["isDebug"].is_boolean()) {
    isDebug = j["isDebug"];
  }
  if (j.contains("randomDeviceID") && j["randomDeviceID"].is_boolean()) {
    randomDeviceID = j["randomDeviceID"];
  }
  return std::nullopt;
}
std::optional<std::string> BypassModule::drawGUI() {
  ImGui::Checkbox("Bypass Debugger", &isDebug);
  ImGui::Checkbox("Random Device ID", &randomDeviceID);
  return std::nullopt;
}
REGISTER_MODULE(BypassModule, BypassModule::getInstance());
} // namespace zr