#pragma once
#include "IL2CPPResolver/Unity/Structures/Ray.hpp"
#include "IL2CPPResolver/Unity/Structures/Vector3.hpp"

namespace Unity {
struct RaycastHit;
enum class QueryTriggerInteraction {
  UseGlobal = 0,
  Ignore = 1,
  Collide = 2,
};
namespace Physics {
bool Raycast(Vector3 origin, Vector3 direction, float maxDistance,
             int layerMask, QueryTriggerInteraction queryTriggerInteraction);
bool Raycast(Vector3 origin, Vector3 direction, float maxDistance,
             int layerMask);
bool Raycast(Vector3 origin, Vector3 direction, float maxDistance);
bool Raycast(Vector3 origin, Vector3 direction);
bool Raycast(Vector3 origin, Vector3 direction, RaycastHit &hitInfo,
             float maxDistance, int layerMask,
             QueryTriggerInteraction queryTriggerInteraction);
bool Raycast(Vector3 origin, Vector3 direction, RaycastHit &hitInfo,
             float maxDistance, int layerMask);
bool Raycast(Vector3 origin, Vector3 direction, RaycastHit &hitInfo,
             float maxDistance);
bool Raycast(Vector3 origin, Vector3 direction, RaycastHit &hitInfo);
bool Raycast(Ray ray, float maxDistance, int layerMask,
             QueryTriggerInteraction queryTriggerInteraction);
bool Raycast(Ray ray, float maxDistance, int layerMask);
bool Raycast(Ray ray, float maxDistance);
bool Raycast(Ray ray);
bool Raycast(Ray ray, RaycastHit &hitInfo, float maxDistance, int layerMask,
             QueryTriggerInteraction queryTriggerInteraction);
bool Raycast(Ray ray, RaycastHit &hitInfo, float maxDistance, int layerMask);
bool Raycast(Ray ray, RaycastHit &hitInfo, float maxDistance);
bool Raycast(Ray ray, RaycastHit &hitInfo);
} // namespace Physics
} // namespace Unity