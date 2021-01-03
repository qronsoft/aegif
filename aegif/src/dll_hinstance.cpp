#include "dll_hinstance.hpp"

// std
#include <mutex>

namespace
{
std::mutex s_mutex;
HINSTANCE s_hInstance = nullptr;
}

namespace aegif
{
HINSTANCE GetDllInstanceHandle()
{
    std::lock_guard lock(s_mutex);
    return s_hInstance;
}

void SetDllInstanceHandle(HINSTANCE hInstance)
{
    std::lock_guard lock(s_mutex);
    if (s_hInstance != nullptr) return;
    s_hInstance = hInstance;
}

void ClearDllInstanceHandle()
{
    std::lock_guard lock(s_mutex);
    s_hInstance = nullptr;
}
}
