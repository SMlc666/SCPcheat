#pragma once

#include "IL2CPPResolver/Unity/API/Component.hpp"
#include "IL2CPPResolver/Unity/Structures/Engine.hpp"
namespace zr {
class PositionControllerAC : public Unity::CComponent {
public:
  Unity::Vector3 getPrevPosition();
};

}; // namespace zr