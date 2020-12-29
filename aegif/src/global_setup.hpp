#pragma once

// AE
#include <AE_GeneralPlug.h>

namespace aegif
{
A_Err GlobalSetup(
    struct SPBasicSuite* pica_basicP,
    A_long major_versionL,
    A_long minor_versionL,
    AEGP_PluginID aegp_pluginId,
    AEGP_GlobalRefcon* global_refconP);
}
