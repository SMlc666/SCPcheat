#include "System_String.hpp"
#include "utf8/utf8.h"
#include <iterator>

static std::string Utf16ToUtf8(wchar_t *utf16String, size_t length) {
  std::string utf8String{};
  utf8String.reserve(length);
  utf8::unchecked::utf16to8(utf16String, utf16String + length,
                            std::back_inserter(utf8String));
  return utf8String;
}

void Unity::System_String::Clear() {

  memset(m_wString, 0, static_cast<size_t>(m_iLength) * 2);
  m_iLength = 0;
}
std::string Unity::System_String::ToString() {

  if (!m_iLength)
    return {};
  return Utf16ToUtf8(m_wString, m_iLength);
}
