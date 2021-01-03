#include "global_setup.hpp"

// AE
#include <AEGP_SuiteHandler.h>

// aegif
#include "version.hpp"
#include "log.hpp"
#include "error_guard.hpp"
#include "aeio.hpp"
#include "catch_exception.hpp"
#include "global_setdown.hpp"

namespace
{
A_Err UnsafeGlobalSetup(
    SPBasicSuite* pica_basicP,
    A_long major_versionL,
    A_long minor_versionL,
    AEGP_PluginID aegp_pluginId,
    AEGP_GlobalRefcon* global_refconP)
{
    aegif::SetupLog(pica_basicP);

    AEGLOG_BLOCK("GlobalSetup");

    AEGLOG_INFO(
        "plugin version: {}.{}.{}.{}.{}",
        AEGIF_MAJOR_VERSION,
        AEGIF_MINOR_VERSION,
        AEGIF_BUG_VERSION,
        AEGIF_STAGE_VERSION,
        AEGIF_BUILD_VERSION);

    AEGP_SuiteHandler suites(pica_basicP);

#ifndef NDEBUG
    GUARD_A_Err(suites.MemorySuite1()->AEGP_SetMemReportingOn(TRUE));
#endif

    AEGLOG_TRACE("register death hook");
    GUARD_A_Err(
        suites.RegisterSuite5()->AEGP_RegisterDeathHook(aegp_pluginId, aegif::GlobalSetdown, nullptr));

    GUARD_A_Err(aegif::RegisterIO(suites, aegp_pluginId));

    return A_Err_NONE;
}
}

namespace aegif
{
A_Err GlobalSetup(
    SPBasicSuite* pica_basicP,
    A_long major_versionL,
    A_long minor_versionL,
    AEGP_PluginID aegp_pluginId,
    AEGP_GlobalRefcon* global_refconP)
{
    return CatchException<UnsafeGlobalSetup>(
        pica_basicP, major_versionL, minor_versionL, aegp_pluginId, global_refconP);
}
}
