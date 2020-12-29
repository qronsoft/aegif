#include "global_setdown.hpp"

// AE
#include "log.hpp"
#include "catch_exception.hpp"

namespace
{
A_Err UnsafeGlobalSetdown(AEGP_GlobalRefcon global_refcon, AEGP_DeathRefcon death_refcon)
{
    AEGLOG_TRACE("GlobalSetdown called");
    aegif::SetdownLog();
    return A_Err_NONE;
}
}

namespace aegif
{
A_Err GlobalSetdown(AEGP_GlobalRefcon global_refcon, AEGP_DeathRefcon death_refcon)
{
    return CatchException<UnsafeGlobalSetdown>(global_refcon, death_refcon);
}
}
