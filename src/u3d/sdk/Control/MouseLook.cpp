#include "MouseLook.hpp"

namespace zr {
float MouseLook::getXSensitivity() {
    return GetMemberValue<float>(0x10);
}

void MouseLook::setXSensitivity(float value) {
    SetMemberValue<float>(0x10, value);
}

float MouseLook::getYSensitivity() {
    return GetMemberValue<float>(0x14);
}

void MouseLook::setYSensitivity(float value) {
    SetMemberValue<float>(0x14, value);
}

bool MouseLook::getClampVerticalRotation() {
    return GetMemberValue<bool>(0x18);
}

void MouseLook::setClampVerticalRotation(bool value) {
    SetMemberValue<bool>(0x18, value);
}

float MouseLook::getMinimumX() {
    return GetMemberValue<float>(0x1c);
}

void MouseLook::setMinimumX(float value) {
    SetMemberValue<float>(0x1c, value);
}

float MouseLook::getMaximumX() {
    return GetMemberValue<float>(0x20);
}

void MouseLook::setMaximumX(float value) {
    SetMemberValue<float>(0x20, value);
}

bool MouseLook::getSmooth() {
    return GetMemberValue<bool>(0x24);
}

void MouseLook::setSmooth(bool value) {
    SetMemberValue<bool>(0x24, value);
}

float MouseLook::getSmoothTime() {
    return GetMemberValue<float>(0x28);
}

void MouseLook::setSmoothTime(float value) {
    SetMemberValue<float>(0x28, value);
}

Unity::Quaternion MouseLook::getCharacterTargetRot() {
    return GetMemberValue<Unity::Quaternion>(0x2c);
}

void MouseLook::setCharacterTargetRot(const Unity::Quaternion &value) {
    SetMemberValue<Unity::Quaternion>(0x2c, value);
}

Unity::Quaternion MouseLook::getCameraTargetRot() {
    return GetMemberValue<Unity::Quaternion>(0x3c);
}

void MouseLook::setCameraTargetRot(const Unity::Quaternion &value) {
    SetMemberValue<Unity::Quaternion>(0x3c, value);
}

bool MouseLook::getCursorIsLocked() {
    return GetMemberValue<bool>(0x4c);
}

void MouseLook::setCursorIsLocked(bool value) {
    SetMemberValue<bool>(0x4c, value);
}
} // namespace zr
