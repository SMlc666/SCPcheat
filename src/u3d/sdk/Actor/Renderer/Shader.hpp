#pragma once
#include "IL2CPPResolver/Unity/API/Object.hpp"
namespace zr {
class Shader : public Unity::CObject {
public:
  static Shader *Find(const std::string &name);
};
} // namespace zr