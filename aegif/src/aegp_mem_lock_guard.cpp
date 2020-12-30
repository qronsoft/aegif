#include "aegp_mem_lock_guard.hpp"

namespace aegif
{
AEGPMemLockGuard::AEGPMemLockGuard(const SPBasicSuite* pica_basicP, AEGP_MemHandle memH)
    : suites_(pica_basicP)
    , memH_(memH)
    , dataPtr_(nullptr)
    , lockError_(A_Err_NONE)
{
    lockError_ = suites_.MemorySuite1()->AEGP_LockMemHandle(memH, &dataPtr_);
    if (lockError_ != A_Err_NONE)
    {
        dataPtr_ = nullptr;
    }
    else if (dataPtr_ == nullptr)
    {
        lockError_ = A_Err_GENERIC;
    }
}
AEGPMemLockGuard::~AEGPMemLockGuard()
{
    Unlock();
}
A_Err AEGPMemLockGuard::Unlock()
{
    if (dataPtr_ == nullptr)
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
