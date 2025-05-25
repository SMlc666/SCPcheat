#include "hook.hpp"
#include <windows.h>
#include <detours.h>

namespace zr {

void Hook::install() {
    if (!m_attached) {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&m_func, m_hook);
        DetourTransactionCommit();
        m_attached = true;
    }
}

void Hook::uninstall() {
    if (m_attached) {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(&m_func, m_hook);
        DetourTransactionCommit();
        m_attached = false;
    }
}

Hook::~Hook() {
    uninstall();
}

} // namespace zr
