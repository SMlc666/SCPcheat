#pragma once
#include <string>
#include <unordered_map>
namespace zr {
enum class ModuleWindowType {
  NONE,
  MOVEMENT,
  COMBAT,
  PLAYER,
  MISC,
  RENDER,
};

static const std::unordered_map<ModuleWindowType, std::string>
    module_window_type_to_string = {{ModuleWindowType::NONE, "NONE"},
                                    {ModuleWindowType::MOVEMENT, "MOVEMENT"},
                                    {ModuleWindowType::COMBAT, "COMBAT"},
                                    {ModuleWindowType::PLAYER, "PLAYER"},
                                    {ModuleWindowType::MISC, "MISC"},
                                    {ModuleWindowType::RENDER, "RENDER"}};

} // namespace zr