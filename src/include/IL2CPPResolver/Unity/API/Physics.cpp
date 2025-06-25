#include "IL2CPPResolver/Unity/API/Physics.hpp"
#include "IL2CPPResolver/API/Class.hpp"
#include "IL2CPPResolver/Unity/Structures/RaycastHit.hpp"
#include "log/log.hpp"

namespace Unity {
namespace Physics {
bool Raycast(Vector3 origin, Vector3 direction, float maxDistance,
             int layerMask, QueryTriggerInteraction queryTriggerInteraction) {
  static void *method = nullptr;
  if (!method) {
    method = IL2CPP::Class::Utils::GetMethodPointer(
        "UnityEngine.Physics", "Raycast",
        {"UnityEngine.Vector3", "UnityEngine.Vector3", "System.Single",
         "System.Int32", "UnityEngine.QueryTriggerInteraction"});
  }
  if (!method) {
    return false;
  }
  return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)(
      Vector3, Vector3, float, int, QueryTriggerInteraction)>(method)(
      origin, direction, maxDistance, layerMask, queryTriggerInteraction);
}

bool Raycast(Vector3 origin, Vector3 direction, float maxDistance,
             int layerMask) {
  static void *method = nullptr;
  if (!method) {
    method = IL2CPP::Class::Utils::GetMethodPointer(
        "UnityEngine.Physics", "Raycast",
        {"UnityEngine.Vector3", "UnityEngine.Vector3", "System.Single",
         "System.Int32"});
  }
  if (!method) {
    return false;
  }
  return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)(
      Vector3, Vector3, float, int)>(method)(origin, direction, maxDistance,
                                             layerMask);
}

bool Raycast(Vector3 origin, Vector3 direction, float maxDistance) {
  static void *method = nullptr;
  if (!method) {
    method = IL2CPP::Class::Utils::GetMethodPointer(
        "UnityEngine.Physics", "Raycast",
        {"UnityEngine.Vector3", "UnityEngine.Vector3", "System.Single"});
  }
  if (!method) {
    return false;
  }
  return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)(
      Vector3, Vector3, float)>(method)(origin, direction, maxDistance);
}

bool Raycast(Vector3 origin, Vector3 direction) {
  static void *method = nullptr;
  if (!method) {
    method = IL2CPP::Class::Utils::GetMethodPointer(
        "UnityEngine.Physics", "Raycast",
        {"UnityEngine.Vector3", "UnityEngine.Vector3"});
  }
  if (!method) {
    return false;
  }
  return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)(Vector3, Vector3)>(
      method)(origin, direction);
}

bool Raycast(Vector3 origin, Vector3 direction, RaycastHit &hitInfo,
             float maxDistance, int layerMask,
             QueryTriggerInteraction queryTriggerInteraction) {
  static void *method = nullptr;
  if (!method) {
    method = IL2CPP::Class::Utils::GetMethodPointer(
        "UnityEngine.Physics", "Raycast",
        {"UnityEngine.Vector3", "UnityEngine.Vector3", "UnityEngine.RaycastHit",
         "System.Single", "System.Int32",
         "UnityEngine.QueryTriggerInteraction"});
  }
  if (!method) {
    return false;
  }
  return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)(
      Vector3, Vector3, RaycastHit *, float, int, QueryTriggerInteraction)>(
      method)(origin, direction, &hitInfo, maxDistance, layerMask,
              queryTriggerInteraction);
}

bool Raycast(Vector3 origin, Vector3 direction, RaycastHit &hitInfo,
             float maxDistance, int layerMask) {
  static void *method = nullptr;
  if (!method) {
    method = IL2CPP::Class::Utils::GetMethodPointer(
        "UnityEngine.Physics", "Raycast",
        {"UnityEngine.Vector3", "UnityEngine.Vector3", "UnityEngine.RaycastHit",
         "System.Single", "System.Int32"});
  }
  if (!method) {
    return false;
  }
  return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)(
      Vector3, Vector3, RaycastHit *, float, int)>(method)(
      origin, direction, &hitInfo, maxDistance, layerMask);
}

bool Raycast(Vector3 origin, Vector3 direction, RaycastHit &hitInfo,
             float maxDistance) {
  static void *method = nullptr;
  if (!method) {
    method = IL2CPP::Class::Utils::GetMethodPointer(
        "UnityEngine.Physics", "Raycast",
        {"UnityEngine.Vector3", "UnityEngine.Vector3", "UnityEngine.RaycastHit",
         "System.Single"});
  }
  if (!method) {
    return false;
  }
  return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)(
      Vector3, Vector3, RaycastHit *, float)>(method)(origin, direction,
                                                      &hitInfo, maxDistance);
}

bool Raycast(Vector3 origin, Vector3 direction, RaycastHit &hitInfo) {
  static void *method = nullptr;
  if (!method) {
    method = IL2CPP::Class::Utils::GetMethodPointer(
        "UnityEngine.Physics", "Raycast",
        {"UnityEngine.Vector3", "UnityEngine.Vector3",
         "UnityEngine.RaycastHit"});
  }
  if (!method) {
    return false;
  }
  return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)(
      Vector3, Vector3, RaycastHit *)>(method)(origin, direction, &hitInfo);
}

bool Raycast(Ray ray, float maxDistance, int layerMask,
             QueryTriggerInteraction queryTriggerInteraction) {
  static void *method = nullptr;
  if (!method) {
    method = IL2CPP::Class::Utils::GetMethodPointer(
        "UnityEngine.Physics", "Raycast",
        {"UnityEngine.Ray", "System.Single", "System.Int32",
         "UnityEngine.QueryTriggerInteraction"});
  }
  if (!method) {
    return false;
  }
  return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)(
      Ray, float, int, QueryTriggerInteraction)>(method)(
      ray, maxDistance, layerMask, queryTriggerInteraction);
}

bool Raycast(Ray ray, float maxDistance, int layerMask) {
  static void *method = nullptr;
  if (!method) {
    method = IL2CPP::Class::Utils::GetMethodPointer(
        "UnityEngine.Physics", "Raycast",
        {"UnityEngine.Ray", "System.Single", "System.Int32"});
  }
  if (!method) {
    return false;
  }
  return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)(Ray, float, int)>(
      method)(ray, maxDistance, layerMask);
}

bool Raycast(Ray ray, float maxDistance) {
  static void *method = nullptr;
  if (!method) {
    method = IL2CPP::Class::Utils::GetMethodPointer(
        "UnityEngine.Physics", "Raycast", {"UnityEngine.Ray", "System.Single"});
  }
  if (!method) {
    return false;
  }
  return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)(Ray, float)>(method)(
      ray, maxDistance);
}

bool Raycast(Ray ray) {
  static void *method = nullptr;
  if (!method) {
    method = IL2CPP::Class::Utils::GetMethodPointer(
        "UnityEngine.Physics", "Raycast", {"UnityEngine.Ray"});
  }
  if (!method) {
    return false;
  }
  return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)(Ray)>(method)(ray);
}

bool Raycast(Ray ray, RaycastHit &hitInfo, float maxDistance, int layerMask,
             QueryTriggerInteraction queryTriggerInteraction) {
  static void *method = nullptr;
  if (!method) {
    method = IL2CPP::Class::Utils::GetMethodPointer(
        "UnityEngine.Physics", "Raycast",
        {"UnityEngine.Ray", "UnityEngine.RaycastHit", "System.Single",
         "System.Int32", "UnityEngine.QueryTriggerInteraction"});
  }
  if (!method) {
    return false;
  }
  return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)(
      Ray, RaycastHit *, float, int, QueryTriggerInteraction)>(method)(
      ray, &hitInfo, maxDistance, layerMask, queryTriggerInteraction);
}

bool Raycast(Ray ray, RaycastHit &hitInfo, float maxDistance, int layerMask) {
  static void *method = nullptr;
  if (!method) {
    method = IL2CPP::Class::Utils::GetMethodPointer(
        "UnityEngine.Physics", "Raycast",
        {"UnityEngine.Ray", "UnityEngine.RaycastHit", "System.Single",
         "System.Int32"});
  }
  if (!method) {
    return false;
  }
  return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)(
      Ray, RaycastHit *, float, int)>(method)(ray, &hitInfo, maxDistance,
                                              layerMask);
}

bool Raycast(Ray ray, RaycastHit &hitInfo, float maxDistance) {
  static void *method = nullptr;
  if (!method) {
    method = IL2CPP::Class::Utils::GetMethodPointer(
        "UnityEngine.Physics", "Raycast",
        {"UnityEngine.Ray", "UnityEngine.RaycastHit", "System.Single"});
  }
  if (!method) {
    return false;
  }
  return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)(
      Ray, RaycastHit *, float)>(method)(ray, &hitInfo, maxDistance);
}

bool Raycast(Ray ray, RaycastHit &hitInfo) {
  static void *method = nullptr;
  if (!method) {
    method = IL2CPP::Class::Utils::GetMethodPointer(
        "UnityEngine.Physics", "Raycast",
        {"UnityEngine.Ray", "UnityEngine.RaycastHit"});
  }
  if (!method) {
    return false;
  }
  return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)(Ray, RaycastHit *)>(
      method)(ray, &hitInfo);
}
} // namespace Physics
} // namespace Unity