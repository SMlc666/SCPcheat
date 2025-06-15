#pragma once


#include "IL2CPPResolver/Unity/Structures/Vector3.hpp"
namespace Unity {


struct Vector4 {
  float x, y, z, w;

  Vector4();
  Vector4(float f1, float f2, float f3, float f4);
};


struct Bounds {
  Vector3 m_vCenter;
  Vector3 m_vExtents;
};

struct Plane {
  Vector3 m_vNormal;
  float fDistance;
};

struct Ray {
  Vector3 m_vOrigin;
  Vector3 m_vDirection;
};

struct Rect {
  float fX, fY;
  float fWidth, fHeight;

  Rect();
  Rect(float f1, float f2, float f3, float f4);
};

struct Color {
  float r, g, b, a;

  Color();
  Color(float fRed = 0.f, float fGreen = 0.f, float fBlue = 0.f,
        float fAlpha = 1.f);
};

struct Matrix4x4 {
  float m[4][4] = {0};

  Matrix4x4() {}

  float *operator[](int i);
};
} // namespace Unity
