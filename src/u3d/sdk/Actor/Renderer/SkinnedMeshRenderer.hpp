#include "IL2CPPResolver/Unity/API/Transform.hpp"
#include "IL2CPPResolver/Unity/Structures/il2cppArray.hpp"
#include "Renderer.hpp"

namespace zr {

class SkinnedMeshRenderer : public Renderer {
public:
  Unity::il2cppArray<Unity::CTransform *> *GetBones();
};

} // namespace zr
