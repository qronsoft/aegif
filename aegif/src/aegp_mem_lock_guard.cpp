#include "aegp_mem_lock_guard.hpp"

namespace aegif
{
AEGPMemLockGuard::AEGPMemLockGuard(const SPBasicSuite* pica_basicP, AEGP_MemHandle memH)
    : suites_(pica_basicP)
    , memH_(memH)
    , dataPtr_(nullptr)
{
}
AEGPMemLockGuard::~AEGPMemLockGuard()
{
    Unlock();
}
A_Err AEGPMemLockGuard::Lock()
{
    if (IsLocked()) return A_Err_NONE;

    if (auto err = suites_.MemorySuite1()->AEGP_LockMemHandle(memH_, &dataPtr_))
    {
        dataPtr_ = nullptr;
        return err;
    }

    return A_Err_NONE;
}
A_Err AEGPMemLockGuard::Unlock()
{
    if (!IsLocked())
    {
        return A_Err_NONE;
    }

    if (auto err = suites_.MemorySuite1()->AEGP_UnlockMemHandle(memH_))
    {
        return err;
    }

    dataPtr_ = nullptr;
    return A_Err_NONE;
}
}
