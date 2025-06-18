#pragma once
#include "IL2CPPResolver/Unity/Structures/Vector3.hpp"
#include <fmt/core.h>
namespace Unity {
struct Ray {
  Vector3 m_Origin{}, m_Direction{};

  inline std::string str() {
    return fmt::format("m_Origin: {}, m_Direction: {}", m_Origin.str(),
                       m_Direction.str());
  }
};
} // namespace Unity