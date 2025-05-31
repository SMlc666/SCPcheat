#pragma once
#include <cstdint>
namespace zr {
void setupU3DModuleBase();
uintptr_t getProcessBase();
uintptr_t getGameAssemblyBase();
uintptr_t getUnityPlayerBase();
} // namespace zr