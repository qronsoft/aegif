#pragma once

// std
#include <string>
#include <type_traits>

// AE
#include <AEGP_SuiteHandler.h>

// aegif
#include "log.hpp"
#include "error_guard.hpp"
#include "aegp_mem_lock_guard.hpp"

namespace aegif
{
template <class T>
A_Err GetMemHData(const SPBasicSuite* pica_basicP, AEGP_MemHandle memH, T* data)
{
    AEGP_SuiteHandler suites(pica_basicP);

    GUARD_ERROR(memH != nullptr, A_Err_GENERIC);

    AEGP_MemSize memSize = 0;
    GUARD_A_Err(suites.MemorySuite1()->AEGP_GetMemHandleSize(memH, &memSize));
    if (memSize != sizeof(T))
    {
        GUARD_A_Err(suites.MemorySuite1()->AEGP_UnlockMemHandle(memH));
        GUARD_A_Err(suites.MemorySuite1()->AEGP_ResizeMemHandle("resize mem handle", sizeof(T), memH));
    }

    aegif::AEGPMemLockGuard lock(pica_basicP, memH);
    GUARD_A_Err(lock.GetLockError());
    if (sizeof(T) > memSize)
    {
        // 0 filling
        memset(lock.Get<char>() + memSize, 0, sizeof(T) - memSize);
    }

    *data = *lock.Get<T>();
    GUARD_A_Err(lock.Unlock());

    return A_Err_NONE;
}

template <class T>
A_Err SetMemHData(const SPBasicSuite* pica_basicP, AEGP_MemHandle memH, T&& data)
{
    AEGP_SuiteHandler suites(pica_basicP);

    GUARD_ERROR(memH != nullptr, A_Err_GENERIC);

    AEGP_MemSize memSize = 0;
    GUARD_A_Err(suites.MemorySuite1()->AEGP_GetMemHandleSize(memH, &memSize));

    if (memSize != sizeof(T))
    {
        GUARD_A_Err(suites.MemorySuite1()->AEGP_UnlockMemHandle(memH));
        GUARD_A_Err(suites.MemorySuite1()->AEGP_ResizeMemHandle("resize mem handle", sizeof(T), memH));
    }

    aegif::AEGPMemLockGuard lock(pica_basicP, memH);
    GUARD_A_Err(lock.GetLockError());
    *lock.Get<std::remove_reference<T>::type>() = std::forward<T>(data);
    GUARD_A_Err(lock.Unlock());

    return A_Err_NONE;
}

template <class Char>
A_Err GetMemHString(const SPBasicSuite* pica_basicP, AEGP_MemHandle memH, std::basic_string<Char>* s)
{
    AEGP_SuiteHandler suites(pica_basicP);

    GUARD_ERROR(memH != nullptr, A_Err_GENERIC);
    aegif::AEGPMemLockGuard lock(pica_basicP, memH);
    GUARD_A_Err(lock.GetLockError());
    s->assign(lock.Get<Char>());
    GUARD_A_Err(lock.Unlock());
    return A_Err_NONE;
}
}
