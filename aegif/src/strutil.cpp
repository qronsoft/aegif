#include "strutil.hpp"

// win32
#include <Windows.h>

template <UINT cp>
std::wstring FromMultiByte(std::string_view s) noexcept
{
    if (s.empty()) return {};
    int resultSize = MultiByteToWideChar(cp, 0, s.data(), (int) s.size(), NULL, 0);
    std::wstring ws(resultSize, 0);
    if (!MultiByteToWideChar(cp, 0, s.data(), (int) s.size(), &ws[0], resultSize)) return {};
    return ws;
}

template <UINT cp>
std::string ToMultiByte(std::wstring_view ws) noexcept
{
    if (ws.empty()) return {};
    int resultSize = WideCharToMultiByte(cp, 0, ws.data(), (int) ws.size(), NULL, 0, NULL, NULL);
    std::string s(resultSize, 0);
    if (!WideCharToMultiByte(cp, 0, ws.data(), (int) ws.size(), &s[0], resultSize, NULL, NULL)) return {};
    return s;
}

namespace aegif
{
std::wstring U8ToW(std::string_view s) noexcept
{
    return FromMultiByte<CP_UTF8>(s);
}

std::string WToU8(std::wstring_view ws) noexcept
{
    return ToMultiByte<CP_UTF8>(ws);
}

std::string OEMToU8(std::string_view s) noexcept
{
    return WToU8(FromMultiByte<CP_OEMCP>(s));
}
}
