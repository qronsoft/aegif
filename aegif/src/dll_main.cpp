// win32
#include <Windows.h>

// aegif
#include "dll_hinstance.hpp"

extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD dwReason, LPVOID lpReserved) try
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        aegif::SetDllInstanceHandle(hinstDll);
        break;
    case DLL_PROCESS_DETACH:
        aegif::ClearDllInstanceHandle();
        break;
    default:
        break;
    }
    return TRUE;
}
catch (...)
{
    return FALSE;
}
