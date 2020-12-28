#pragma once

// std
#include <string>
#include <string_view>

namespace aegif
{
std::wstring U8ToW(std::string_view s) noexcept;
std::string WToU8(std::wstring_view ws) noexcept;
std::string OEMToU8(std::string_view s) noexcept;
}
