#pragma once

#include "IL2CPPResolver/Unity/API/Object.hpp"

namespace Unity {
class CGameObject;
class CTransform;
}
namespace Unity {
struct ComponentFunctions_t {
  void *m_GetGameObject = nullptr;
  void *m_GetTransform = nullptr;
};
extern ComponentFunctions_t m_ComponentFunctions;

class CComponent : public CObject {
public:
  CGameObject *GetGameObject();

  CTransform *GetTransform();
};

namespace Component {
void Initialize();
} // namespace Component
} // namespace Unity