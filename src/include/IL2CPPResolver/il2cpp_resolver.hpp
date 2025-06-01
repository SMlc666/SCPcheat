#pragma once

// Default Headers

#define _USE_MATH_DEFINES
#include <Windows.h>
#include <math.h>

// Application Defines
#ifndef UNITY_VERSION_2022_3_8F1
// If Unity version is equal or greater than 2022.3.8f1 uncomment this define.
// #define UNITY_VERSION_2022_3_8F1
#endif

// IL2CPP Defines

// Disable Asserts
// #define IL2CPP_ASSERT(x) assert(0)
#ifndef IL2CPP_ASSERT
#include <cassert>
#define IL2CPP_ASSERT(x) assert(x)
#endif

#ifndef IL2CPP_RStr
// If you wanna forward to some string encryption just define before including
// this file.
#define IL2CPP_RStr(x) x
#endif

#ifndef IL2CPP_MAIN_MODULE
// If the game for some reason uses diff module name just define own one before
// including this file.
#define IL2CPP_MAIN_MODULE IL2CPP_RStr("GameAssembly.dll")
#endif

// Unity Class APIs - So they're accessible everywhere
namespace Unity {
class CCamera;
class CComponent;
class CGameObject;
class CLayerMask;
class CObject;
class CRigidbody;
class CTransform;
} // namespace Unity

namespace IL2CPP {
namespace UnityAPI {
enum m_eExportObfuscationType {
  None = 0,
  ROT = 1,
  MAX = 2,
};
extern m_eExportObfuscationType m_ExportObfuscation;

extern int m_ROTObfuscationValue;
void *ResolveExport(const char *m_Name);

bool ResolveExport_Boolean(void **m_Address, const char *m_Name);

bool Initialize();
} // namespace UnityAPI

/*
 *	You need to call this, before using any IL2CPP/Unity Functions!
 *	Args:
 *		m_WaitForModule - Will wait for main module if you're loading
 *your dll earlier than the main module. m_MaxSecondsWait - Max seconds it will
 *wait for main module to load otherwise will return false to prevent infinite
 *loop.
 */
bool Initialize(bool m_WaitForModule = false, int m_MaxSecondsWait = 60);
} // namespace IL2CPP
