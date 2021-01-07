#pragma once

// AE
#include <AEGP_SuiteHandler.h>

namespace aegif
{
class AEGPMemLockGuard
{
public:
    AEGPMemLockGuard(const SPBasicSuite* pica_basicP, AEGP_MemHandle memH);
    ~AEGPMemLockGuard();
    A_Err Lock();
    A_Err Unlock();
    template <class T>
    T* Get()
    {
        return reinterpret_cast<T*>(dataPtr_);
    }
    template <class T>
    const T* Get() const
    {
        return reinterpret_cast<const T*>(dataPtr_);
    }
    bool IsLocked() const { return dataPtr_ != nullptr; }

private:
    AEGP_SuiteHandler suites_;
    AEGP_MemHandle memH_;
    void* dataPtr_;
};
}
