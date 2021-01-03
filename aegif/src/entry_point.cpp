// AE
#include <AEConfig.h>
#include <entry.h>

// aegif
#include "global_setup.hpp"

extern "C" DllExport A_Err EntryPointFunc(
    struct SPBasicSuite* pica_basicP,
    A_long major_versionL,
    A_long minor_versionL,
    AEGP_PluginID aegp_pluginId,
    AEGP_GlobalRefcon* global_refconP) try
{
    return aegif::GlobalSetup(pica_basicP, major_versionL, minor_versionL, aegp_pluginId, global_refconP);
}
catch (...)
{
    return A_Err_GENERIC;
}
