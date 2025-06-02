#include "MainThread.hpp"
#include "draw/draw.hpp"
#include "module/ModuleSetup.hpp"
#include "u3d/u3d.hpp"
#include <thread>
#include <windows.h>

void zr::createMainThread() {
  std::thread([]() {
    zr::setupU3D();
    zr::setupDraw();
    zr::setupModules();
  }).detach();
}