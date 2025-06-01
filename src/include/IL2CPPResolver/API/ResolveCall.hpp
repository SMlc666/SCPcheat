#pragma once

namespace IL2CPP {
// Without this function, you're pretty much fucked up.
void *ResolveCall(const char *m_Name);
} // namespace IL2CPP