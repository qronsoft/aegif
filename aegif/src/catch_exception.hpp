#pragma once

// AE
#include <A.h>

// aegif
#include "charenc.hpp"
#include "log.hpp"

namespace aegif
{
template <void* Fn, class... Args>
A_Err CatchException(Args... args) try
{
    return Fn(args...);
}
catch (const std::exception& ex)
{
    AEGLOG_ERROR("catch exception: {}", aegif::OEMToU8(ex.what()));
    return A_Err_GENERIC;
}
catch (A_Err err)
{
    AEGLOG_ERROR("catch A_Err: {}", err);
    return err;
}
catch (...)
{
    AEGLOG_ERROR("catch unknown exception");
    return A_Err_GENERIC;
}
}
