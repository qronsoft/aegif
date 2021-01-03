#pragma once

// win32
#include <Windows.h>

namespace aegif
{
HINSTANCE GetDllInstanceHandle();
void SetDllInstanceHandle(HINSTANCE hInstance);
void ClearDllInstanceHandle();
}
