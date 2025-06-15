#include "Engine.hpp"
#include "IL2CPPResolver/IL2CPP_Resolver.hpp"
#include "Quaternion.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"

namespace Unity {
constexpr float floatInf = std::numeric_limits<float>::infinity();

const Vector2 Vector2::positiveInfinity = {floatInf, floatInf};
const Vector2 Vector2::negativeInfinity = {-floatInf, -floatInf};
const Vector2 Vector2::down = {0.f, -1.f};
const Vector2 Vector2::left = {-1.f, 0.f};
const Vector2 Vector2::one = {1.f, 1.f};
const Vector2 Vector2::right = {1.f, 0.f};
const Vector2 Vector2::up = {0.f, 1.f};
const Vector2 Vector2::zero = {0.f, 0.f};

Vector2::operator Vector3() const { return {x, y, 0}; }

const Vector3 Vector3::positiveInfinity = {floatInf, floatInf, floatInf};
const Vector3 Vector3::negativeInfinity = {-floatInf, -floatInf, -floatInf};
const Vector3 Vector3::back = {0.f, 0.f, -1.f};
const Vector3 Vector3::down = {0.f, -1.f, 0.f};
const Vector3 Vector3::forward = {0.f, 0.f, 1.f};
const Vector3 Vector3::left = {-1.f, 0.f, 0.f};
const Vector3 Vector3::one = {1.f, 1.f, 1.f};
const Vector3 Vector3::right = {1.f, 0.f, 0.f};
const Vector3 Vector3::up = {0.f, 1.f, 0.f};
const Vector3 Vector3::zero = {0.f, 0.f, 0.f};

const Quaternion Quaternion::identity = {0.f, 0.f, 0.f, 1.f};
} // namespace Unity
Unity::Vector4::Vector4() { x = y = z = w = 0.f; }
Unity::Vector4::Vector4(float f1, float f2, float f3, float f4) {
  x = f1;
  y = f2;
  z = f3;
  w = f4;
}

Unity::Rect::Rect() { fX = fY = fWidth = fHeight = 0.f; }
Unity::Rect::Rect(float f1, float f2, float f3, float f4) {
  fX = f1;
  fY = f2;
  fWidth = f3;
  fHeight = f4;
}
Unity::Color::Color() { r = g = b = a = 0.f; }
Unity::Color::Color(float fRed, float fGreen, float fBlue, float fAlpha) {
  r = fRed;
  g = fGreen;
  b = fBlue;
  a = fAlpha;
}
float *Unity::Matrix4x4::operator[](int i) { return m[i]; }
