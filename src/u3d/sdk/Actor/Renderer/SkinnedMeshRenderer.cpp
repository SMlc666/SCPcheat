#include "SkinnedMeshRenderer.hpp"

namespace zr {
Unity::il2cppArray<Unity::CTransform *> *SkinnedMeshRenderer::GetBones() {
  return CallMethod<Unity::il2cppArray<Unity::CTransform *> *>("get_bones");
}
} // namespace zr