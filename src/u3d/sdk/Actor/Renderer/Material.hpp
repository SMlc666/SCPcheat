#pragma once

#include "IL2CPPResolver/Unity/API/Object.hpp"
#include "u3d/sdk/Actor/Renderer/Shader.hpp"
namespace zr {
class Material : public Unity::CObject {
public:
  Shader *getShader();
  void setShader(Shader *shader);
  void setInt(const std::string &name, int value);
};
} // namespace zr