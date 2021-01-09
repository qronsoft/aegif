#include "aegp_mem.hpp"

namespace aegif
{
A_Err GetMemHData(const SPBasicSuite* pica_basicP, AEGP_MemHandle memH, std::vector<char>* data)
{
    GUARD_ERROR(data != nullptr, A_Err_GENERIC);
    data->clear();

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
    data->assign(lock.Get<char>(), lock.Get<char>() + memSize);
    GUARD_A_Err(lock.Unlock());

    return A_Err_NONE;
}
A_Err SetMemHData(const SPBasicSuite* pica_basicP, AEGP_MemHandle memH, const std::vector<char>& data)
{
    GUARD_ERROR(memH != nullptr, A_Err_GENERIC);

    AEGP_SuiteHandler suites(pica_basicP);

    AEGP_MemSize memSize = 0;
    GUARD_A_Err(suites.MemorySuite1()->AEGP_GetMemHandleSize(memH, &memSize));

    if (memSize != data.size())
    {
        GUARD_A_Err(suites.MemorySuite1()->AEGP_UnlockMemHandle(memH));
        GUARD_A_Err(suites.MemorySuite1()->AEGP_ResizeMemHandle("resize mem handle", data.size(), memH));
    }

    aegif::AEGPMemLockGuard lock(pica_basicP, memH);
    GUARD_A_Err(lock.Lock());
    GUARD_ERROR(lock.IsLocked(), A_Err_GENERIC);
    memcpy(lock.Get<char>(), data.data(), data.size());
    GUARD_A_Err(lock.Unlock());

    return A_Err_NONE;
}
}
