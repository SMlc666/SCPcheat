#pragma once

#include "IL2CPPResolver/Unity/API/GameObject.hpp"
#include "IL2CPPResolver/Unity/Structures/Engine.hpp"
namespace Unity {
struct CameraFunctions_t {
  void *m_GetCurrent = nullptr;
  void *m_GetMain = nullptr;
  void *m_GetDepth = nullptr;
  void *m_SetDepth = nullptr;
  void *m_GetFieldOfView = nullptr;
  void *m_SetFieldOfView = nullptr;
  void *m_WorldToScreen = nullptr;
};
extern CameraFunctions_t m_CameraFunctions;

class CCamera : public CGameObject {
public:
  float GetDepth();

  void SetDepth(float m_fValue);

  float GetFieldOfView();

  void SetFieldOfView(float m_fValue);

  void WorldToScreen(Vector3 &m_vWorld, Vector3 &m_vScreen, int m_iEye = 2);
};

namespace Camera {
void Initialize();

CCamera *GetCurrent();

CCamera *GetMain();
} // namespace Camera

enum m_eCameraType : int {
  m_eCameraType_Game = 1,
  m_eCameraType_SceneView = 2,
  m_eCameraType_Preview = 4,
  m_eCameraType_VR = 8,
  m_eCameraType_Reflection = 16,
};

enum m_eCameraEye : int {
  m_eCameraEye_Left = 0,
  m_eCameraEye_Right = 1,
  m_eCameraEye_Center = 2,
};
} // namespace Unity