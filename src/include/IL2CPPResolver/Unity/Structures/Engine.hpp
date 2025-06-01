#pragma once


namespace Unity {
struct Vector2 {
  float x, y;

  Vector2();
  Vector2(float f1, float f2);
};

struct Vector3 {
  float x, y, z;

  Vector3();
  Vector3(float f1, float f2, float f3);

  float Length();

  float Dot(Vector3 b);

  Vector3 Normalize();
  void ToVectors(Vector3 *m_pForward, Vector3 *m_pRight, Vector3 *m_pUp);
};

struct Vector4 {
  float x, y, z, w;

  Vector4();
  Vector4(float f1, float f2, float f3, float f4);
};

struct Quaternion {
  float x, y, z, w;

  Quaternion();
  Quaternion(float f1, float f2, float f3, float f4);

  Quaternion Euler(float m_fX, float m_fY, float m_fZ);

  Quaternion Euler(Vector3 m_vRot);

  Vector3 ToEuler();
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
