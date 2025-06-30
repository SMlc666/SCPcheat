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
#include "u3d/sdk/Actor/Player/Player.hpp"
#include "u3d/sdk/PlayerUpdate.hpp"
#include <Windows.h>
#include <random>
namespace {
safetyhook::InlineHook IsDebuggerPresent_hook;
safetyhook::InlineHook PluginAuthenicator_GetHash_hook;
safetyhook::InlineHook ScriptHelper_GetActivePlatform_hook;
safetyhook::InlineHook JObject_set_Item_hook;
safetyhook::InlineHook get_platform_hook;
Unity::System_String *ScriptHelper_GetActivePlatform_Proxy() {
  if (zr::BypassModule::getInstance()->spoofAndroid) {
    return IL2CPP::String::New("Android");
  }
  auto ret = ScriptHelper_GetActivePlatform_hook.call<Unity::System_String *>();
  zr::BypassModule::getInstance()->getLogger().info(
      "ScriptHelper_GetActivePlatform: {}", ret->ToString());
  return ret;
}
int UnityEngine_Application_get_platform() {
  if (zr::BypassModule::getInstance()->spoofAndroid) {
    return 11;
  }

  return get_platform_hook.call<int>();
}
void JObject_set_Item_Proxy(void *__this, Unity::System_String *propertyName,
                            void *value) {
  zr::BypassModule::getInstance()->getLogger().info(
      "JObject::set_Item: propertyName = {}", propertyName->ToString());
  static void *op_Implicit_ptr = IL2CPP::Class::Utils::GetMethodPointer(
      "Newtonsoft.Json.Linq.JToken", "op_Implicit", {"System.String"});
  if (propertyName->ToString() == "platform" &&
      zr::BypassModule::getInstance()->spoofAndroid) {
    if (op_Implicit_ptr) {
      void *jtoken = reinterpret_cast<void *(
          UNITY_CALLING_CONVENTION)(Unity::System_String *)>(op_Implicit_ptr)(
          IL2CPP::String::New("Android"));
      JObject_set_Item_hook.call<void>(__this, propertyName, jtoken);
      return;
    } else {
      zr::BypassModule::getInstance()->getLogger().error(
          "op_Implicit not found");
    }
  }
  JObject_set_Item_hook.call<void>(__this, propertyName, value);
}
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

std::optional<std::string> BypassModule::enable() {
  playerUpdateCallbacksHandle =
      playerUpdateCallbacks.append([this](Player *player) {
        if (isEnabled() && alwaysRandomAccount) {
          static std::random_device rd;
          static std::mt19937 gen(rd());
          std::uniform_int_distribution<> dis(0, 10000000);
          int64_t accountID = dis(gen);
          const std::string charset =
              "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
          std::string randomStr;
          std::uniform_int_distribution<> dis2(0, charset.size() - 1);
          for (int i = 0; i < 10; ++i) {
            randomStr += charset[dis2(gen)];
          }
          player->syncAccount(accountID, randomStr, randomStr, randomStr);
        }
      });
  return std::nullopt;
}
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
  void *ScriptHelper_GetActivePlatform_ptr =
      IL2CPP::Class::Utils::GetMethodPointer("ScriptHelper",
                                             "GetActivePlatform", 0);
  if (ScriptHelper_GetActivePlatform_ptr == nullptr) {
    return "Failed to get ScriptHelper_GetActivePlatform";
  } else {
    getLogger().info("ScriptHelper_GetActivePlatform: {:p}",
                     (void *)ScriptHelper_GetActivePlatform_ptr);
  }
  ScriptHelper_GetActivePlatform_hook = safetyhook::create_inline(
      ScriptHelper_GetActivePlatform_ptr, ScriptHelper_GetActivePlatform_Proxy);

  void *JObject_set_Item_ptr = IL2CPP::Class::Utils::GetMethodPointer(
      "Newtonsoft.Json.Linq.JObject", "set_Item", 2);
  if (JObject_set_Item_ptr == nullptr) {
    return "Failed to get JObject_set_Item";
  } else {
    getLogger().info("JObject_set_Item: {:p}", (void *)JObject_set_Item_ptr);
  }
  JObject_set_Item_hook =
      safetyhook::create_inline(JObject_set_Item_ptr, JObject_set_Item_Proxy);
  void *get_platform_ptr = IL2CPP::Class::Utils::GetMethodPointer(
      "UnityEngine.Application", "get_platform", 0);
  if (get_platform_ptr == nullptr) {
    return "Failed to get get_platform";
  } else {
    getLogger().info("get_platform: {:p}", (void *)get_platform_ptr);
  }
  get_platform_hook = safetyhook::create_inline(
      get_platform_ptr, UnityEngine_Application_get_platform);
  return std::nullopt;
}

std::optional<std::string> BypassModule::toJson(nlohmann::json &j) const {
  j["isDebug"] = isDebug;
  j["randomDeviceID"] = randomDeviceID;
  j["alwaysRandomAccount"] = alwaysRandomAccount;
  j["spoofAndroid"] = spoofAndroid;
  return std::nullopt;
}
std::optional<std::string> BypassModule::fromJson(const nlohmann::json &j) {
  if (j.contains("isDebug") && j["isDebug"].is_boolean()) {
    isDebug = j["isDebug"];
  }
  if (j.contains("randomDeviceID") && j["randomDeviceID"].is_boolean()) {
    randomDeviceID = j["randomDeviceID"];
  }
  if (j.contains("alwaysRandomAccount") &&
      j["alwaysRandomAccount"].is_boolean()) {
    alwaysRandomAccount = j["alwaysRandomAccount"];
  }
  if (j.contains("spoofAndroid") && j["spoofAndroid"].is_boolean()) {
    spoofAndroid = j["spoofAndroid"];
  }
  return std::nullopt;
}
std::optional<std::string> BypassModule::drawGUI() {
  ImGui::Checkbox("Bypass Debugger", &isDebug);
  ImGui::Checkbox("Random Device ID", &randomDeviceID);
  ImGui::Checkbox("Always Random Account", &alwaysRandomAccount);
  ImGui::Checkbox("Spoof Android", &spoofAndroid);
  return std::nullopt;
}
REGISTER_MODULE(BypassModule, BypassModule::getInstance());
} // namespace zr