#pragma once

#include "IL2CPPResolver/Unity/API/Object.hpp"
#include "IL2CPPResolver/Unity/Structures/Quaternion.hpp"

/*
class UnityStandardAssets.Characters.FirstPerson.MouseLook : System.Object
{
    System.Single XSensitivity; // 0x10
    System.Single YSensitivity; // 0x14
    System.Boolean clampVerticalRotation; // 0x18
    System.Single MinimumX; // 0x1c
    System.Single MaximumX; // 0x20
    System.Boolean smooth; // 0x24
    System.Single smoothTime; // 0x28
    UnityEngine.Quaternion m_CharacterTargetRot; // 0x2c
    UnityEngine.Quaternion m_CameraTargetRot; // 0x3c
    System.Boolean m_cursorIsLocked; // 0x4c
    System.Void Init(UnityEngine.Transform character, UnityEngine.Transform
camera); // 0x003e3f10 System.Void LookRotation(UnityEngine.Transform character,
UnityEngine.Transform camera); // 0x003e3f70 System.Void UpdateCursorLock(); //
0x003e44f0 UnityEngine.Quaternion
ClampRotationAroundXAxis(UnityEngine.Quaternion q); // 0x003e3e50 System.Void
.ctor(); // 0x003e45a0
}
    */
namespace zr {
class MouseLook : public Unity::CObject {
public:
  float getXSensitivity();
  void setXSensitivity(float value);
  float getYSensitivity();
  void setYSensitivity(float value);
  bool getClampVerticalRotation();
  void setClampVerticalRotation(bool value);
  float getMinimumX();
  void setMinimumX(float value);
  float getMaximumX();
  void setMaximumX(float value);
  bool getSmooth();
  void setSmooth(bool value);
  float getSmoothTime();
  void setSmoothTime(float value);
  Unity::Quaternion getCharacterTargetRot();
  void setCharacterTargetRot(const Unity::Quaternion &value);
  Unity::Quaternion getCameraTargetRot();
  void setCameraTargetRot(const Unity::Quaternion &value);
  bool getCursorIsLocked();
  void setCursorIsLocked(bool value);
};
} // namespace zr
