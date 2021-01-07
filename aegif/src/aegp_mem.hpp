#pragma once

// std
#include <string>
#include <vector>

// AE
#include <AEGP_SuiteHandler.h>

// aegif
#include "log.hpp"
#include "error_guard.hpp"
#include "aegp_mem_lock_guard.hpp"

namespace aegif
{
A_Err GetMemHData(const SPBasicSuite* pica_basicP, AEGP_MemHandle memH, std::vector<char>* data);
A_Err SetMemHData(const SPBasicSuite* pica_basicP, AEGP_MemHandle memH, const std::vector<char>& data);

template <class Char>
A_Err GetMemHString(const SPBasicSuite* pica_basicP, AEGP_MemHandle memH, std::basic_string<Char>* s)
{
    GUARD_ERROR(s != nullptr, A_Err_GENERIC);
    s->clear();

    if (memH == nullptr)
    {
        return A_Err_NONE;
    }

    AEGP_SuiteHandler suites(pica_basicP);

    AEGP_MemSize memSize = 0;
    GUARD_A_Err(suites.MemorySuite1()->AEGP_GetMemHandleSize(memH, &memSize));

    aegif::AEGPMemLockGuard lock(pica_basicP, memH);
    GUARD_A_Err(lock.Lock());
    GUARD_ERROR(lock.IsLocked(), A_Err_GENERIC);
    s->assign(lock.Get<Char>(), lock.Get<Char>() + memSize / sizeof(Char));
    GUARD_A_Err(lock.Unlock());
    return A_Err_NONE;
}
}
