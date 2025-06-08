#pragma once

#include "IL2CPPResolver/Unity/API/Component.hpp"
namespace zr {
class Rooms : public Unity::CComponent {
public:
  static std::vector<Rooms *> getAll();
  std::string getName();
};
} // namespace zr