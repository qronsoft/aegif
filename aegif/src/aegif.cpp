// AE
#include <AEConfig.h>
#include <entry.h>
#include <AEGP_SuiteHandler.h>

// gifski
#include <gifski.h>

// aegif
#include "log.hpp"
#include "strutil.hpp"
#include "scopeguard.hpp"

namespace
{
#define CATCH_EXCEPTION                                                                                      \
    catch (const std::exception& ex)                                                                         \
    {                                                                                                        \
        AEGLOG_ERROR("catch exception: {}", aegif::OEMToU8(ex.what()));                                      \
        return A_Err_GENERIC;                                                                                \
    }                                                                                                        \
    catch (A_Err err) { return err; }                                                                        \
    catch (...)                                                                                              \
    {                                                                                                        \
        AEGLOG_ERROR("catch unknown exception");                                                             \
        return A_Err_GENERIC;                                                                                \
    }

A_Err DeathHook(AEGP_GlobalRefcon global_refcon, AEGP_DeathRefcon death_refcon) try
{
    AEGLOG_TRACE("DeathHook called");

    aegif::SetdownLog();
    return A_Err_NONE;
}
CATCH_EXCEPTION
}

extern "C" DllExport A_Err EntryPointFunc(
    struct SPBasicSuite* pica_basicP,
    A_long major_versionL,
    A_long minor_versionL,
    AEGP_PluginID aegp_pluginId,
    AEGP_GlobalRefcon* global_refconP) try
{
    aegif::SetupLog(pica_basicP);

    AEGLOG_BLOCK("EntryPointFunc");

    AEGP_SuiteHandler suites(pica_basicP);

    AEGLOG_TRACE("register death hook");
    suites.RegisterSuite5()->AEGP_RegisterDeathHook(aegp_pluginId, DeathHook, nullptr);

    GifskiSettings gifskiSettings = {};
    gifski* gifski                = gifski_new(&gifskiSettings);
    if (gifski == nullptr)
    {
        AEGLOG_ERROR("invalid gifski settings");
    }
    aegif::ScopeGuard setdownGifski([&]() {
        if (gifski)
        {
            gifski_finish(gifski);
            gifski = nullptr;
        }
    });

    return A_Err_NONE;
}
CATCH_EXCEPTION
