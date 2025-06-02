#include "LocalPlayer.hpp"

zr::LocalPlayer *zr::LocalPlayer::getInstance() {
  void *LocalPlayerPtr =
      IL2CPP::Class::Utils::GetStaticField("PlayerUtilities", "localPlayer");
  return reinterpret_cast<zr::LocalPlayer *>(LocalPlayerPtr);
}