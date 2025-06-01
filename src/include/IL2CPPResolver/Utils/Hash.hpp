#pragma once

#include <cstdint>
namespace IL2CPP
{
	namespace Utils
	{
        namespace Hash
        {
        uint32_t Get(const char *m_String);

        constexpr uint32_t GetCompileTime(const char *m_String);
        }
	}
}

#define IL2CPP_HASH(m_String) \
[](){ \
    static constexpr uint32_t m_Hash = IL2CPP::Utils::Hash::GetCompileTime(m_String); \
    return m_Hash; \
}()
