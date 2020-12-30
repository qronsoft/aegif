#pragma once

// std
#include <string>
#include <string_view>

// AE
#include <A.h>

namespace aegif
{
std::wstring U8ToW(std::string_view s) noexcept;
std::string WToU8(std::wstring_view ws) noexcept;
std::string OEMToU8(std::string_view s) noexcept;
std::string U16ToU8(std::basic_string_view<A_UTF16Char> s) noexcept;
}
