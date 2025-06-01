#pragma once

#include "IL2CPPResolver/Unity/Structures/System_String.hpp"
#include <string>
namespace IL2CPP {
namespace String {
Unity::System_String *New(const char *m_String);

Unity::System_String *New(std::string m_String);

namespace NoGC {
Unity::System_String *New(const char *m_String);

Unity::System_String *New(std::string m_String);
} // namespace NoGC
} // namespace String
} // namespace IL2CPP