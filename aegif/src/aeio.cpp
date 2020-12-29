#include "aeio.hpp"

// AE
#include <AE_Macros.h>

// aegif
#include "log.hpp"
#include "charenc.hpp"
#include "error_guard.hpp"
#include "catch_exception.hpp"

namespace
{
A_Err AEIO_InitOutputSpec(AEIO_BasicData* basic_dataP, AEIO_OutSpecH outH, A_Boolean* user_interacted)
{
    AEGLOG_BLOCK("AEIO_InitOutputSpec");
    // TODO: impl
    return AEIO_Err_USE_DFLT_CALLBACK;
}
A_Err AEIO_GetDepths(AEIO_BasicData* basic_dataP, AEIO_OutSpecH outH, AEIO_SupportedDepthFlags* which)
{
    AEGLOG_BLOCK("AEIO_GetDepths");
    *which = AEIO_SupportedDepthFlags_NONE;
    *which |= AEIO_SupportedDepthFlags_DEPTH_24;
    *which |= AEIO_SupportedDepthFlags_DEPTH_32;
    return A_Err_NONE;
}

A_Err AEIO_UserOptionsDialog(
    AEIO_BasicData* basic_dataP,
    AEIO_OutSpecH outH,
    const PF_EffectWorld* sample0,
    A_Boolean* user_interacted0)
{
    AEGLOG_BLOCK("AEIO_UserOptionsDialog");
    // TODO: impl
    return AEIO_Err_USE_DFLT_CALLBACK;
}

A_Err AEIO_StartAdding(AEIO_BasicData* basic_dataP, AEIO_OutSpecH outH, A_long flags)
{
    AEGLOG_BLOCK("AEIO_StartAdding");
    // TODO: impl
    return AEIO_Err_USE_DFLT_CALLBACK;
}

A_Err AEIO_AddFrame(
    AEIO_BasicData* basic_dataP,
    AEIO_OutSpecH outH,
    A_long frame_index,
    A_long frames,
    const PF_EffectWorld* wP,
    const A_LPoint* origin0,
    A_Boolean was_compressedB,
    AEIO_InterruptFuncs* inter0)
{
    AEGLOG_BLOCK("AEIO_AddFrame");

    // TODO: impl
    return AEIO_Err_USE_DFLT_CALLBACK;
}

A_Err AEIO_EndAdding(AEIO_BasicData* basic_dataP, AEIO_OutSpecH outH, A_long flags)
{
    AEGLOG_BLOCK("AEIO_EndAdding");
    // TODO: impl
    return AEIO_Err_USE_DFLT_CALLBACK;
}

A_Err ConstructModuleInfo(AEGP_SuiteHandler& suites, AEIO_ModuleInfo* info)
{
    AEGLOG_BLOCK("ConstructModuleInfo");

    AEFX_CLR_STRUCT(*info);

    info->sig = 'qAEG';
    strcpy(info->name, "Ae GIF");

    info->flags = AEIO_MFlag_NONE;
    info->flags |= AEIO_MFlag_OUTPUT;
    info->flags |= AEIO_MFlag_FILE;
    info->flags |= AEIO_MFlag_VIDEO;
    info->flags |= AEIO_MFlag_CAN_ADD_FRAMES_NON_LINEAR;

    info->flags2 = AEIO_MFlag2_NONE;

    info->max_width  = 0xffff;
    info->max_height = 0xffff;

    info->num_filetypes  = 1;
    info->num_extensions = 1;
    info->num_clips      = 0;

    info->create_kind.type    = 'GIFf';
    info->create_kind.creator = AEIO_ANY_CREATOR;

    info->create_ext.pad          = '.';
    info->create_ext.extension[0] = 'g';
    info->create_ext.extension[1] = 'i';
    info->create_ext.extension[2] = 'f';

    info->read_kinds[0].mac.type    = 'GIFf';
    info->read_kinds[0].mac.creator = AEIO_ANY_CREATOR;
    info->read_kinds[1].ext         = info->create_ext;

    info->num_aux_extensionsS = 0;

    return A_Err_NONE;
}

A_Err ConstructFunctionBlock(AEGP_SuiteHandler& suites, AEIO_FunctionBlock4* funcs)
{
    AEGLOG_BLOCK("ConstructFunctionBlock");

    AEFX_CLR_STRUCT(*funcs);

#define DEFAULT_CALLBACK(callbackName)                                                                       \
    [](auto...) -> A_Err {                                                                                   \
        AEGLOG_TRACE("default {} called", #callbackName);                                                    \
        return AEIO_Err_USE_DFLT_CALLBACK;                                                                   \
    }

    /* required */
    funcs->AEIO_InitOutputSpec       = aegif::CatchException<AEIO_InitOutputSpec>;
    funcs->AEIO_GetFlatOutputOptions = DEFAULT_CALLBACK(AEIO_GetFlatOutputOptions);
    funcs->AEIO_SetOutputFile        = DEFAULT_CALLBACK(AEIO_SetOutputFile);
    funcs->AEIO_StartAdding          = aegif::CatchException<AEIO_StartAdding>;
    funcs->AEIO_AddFrame             = aegif::CatchException<AEIO_AddFrame>;
    funcs->AEIO_EndAdding            = aegif::CatchException<AEIO_EndAdding>;
    funcs->AEIO_WriteLabels          = DEFAULT_CALLBACK(AEIO_WriteLabels);
    funcs->AEIO_GetSizes             = DEFAULT_CALLBACK(AEIO_GetSizes);
    funcs->AEIO_GetDepths            = aegif::CatchException<AEIO_GetDepths>;
    funcs->AEIO_GetOutputSuffix      = DEFAULT_CALLBACK(AEIO_GetOutputSuffix);

    /* not required */
    funcs->AEIO_DisposeOutputOptions = DEFAULT_CALLBACK(AEIO_DisposeOutputOptions);
    funcs->AEIO_UserOptionsDialog    = aegif::CatchException<AEIO_UserOptionsDialog>;
    funcs->AEIO_GetOutputInfo        = DEFAULT_CALLBACK(AEIO_GetOutputInfo);
    funcs->AEIO_OutputInfoChanged    = DEFAULT_CALLBACK(AEIO_OutputInfoChanged);
    funcs->AEIO_Flush                = DEFAULT_CALLBACK(AEIO_Flush);
    funcs->AEIO_Idle                 = nullptr;
    funcs->AEIO_SetUserData          = DEFAULT_CALLBACK(AEIO_SetUserData);
    funcs->AEIO_GetMimeType          = DEFAULT_CALLBACK(AEIO_GetMimeType);

    /* not used for gif */
    funcs->AEIO_OutputFrame            = nullptr;
    funcs->AEIO_AddMarker              = nullptr;
    funcs->AEIO_AddMarker2             = nullptr;
    funcs->AEIO_AddMarker3             = nullptr;
    funcs->AEIO_UserAudioOptionsDialog = nullptr;
    funcs->AEIO_AddSoundChunk          = nullptr;

    /* input only callbacks, not used */
    funcs->AEIO_InitInSpecFromFile    = nullptr;
    funcs->AEIO_InitInSpecInteractive = nullptr;
    funcs->AEIO_DisposeInSpec         = nullptr;
    funcs->AEIO_FlattenOptions        = nullptr;
    funcs->AEIO_InflateOptions        = nullptr;
    funcs->AEIO_SynchInSpec           = nullptr;
    funcs->AEIO_GetActiveExtent       = nullptr;
    funcs->AEIO_GetInSpecInfo         = nullptr;
    funcs->AEIO_DrawSparseFrame       = nullptr;
    funcs->AEIO_GetDimensions         = nullptr;
    funcs->AEIO_GetDuration           = nullptr;
    funcs->AEIO_GetTime               = nullptr;
    funcs->AEIO_GetSound              = nullptr;
    funcs->AEIO_InqNextFrameTime      = nullptr;
    funcs->AEIO_SeqOptionsDlg         = nullptr;
    funcs->AEIO_GetNumAuxChannels     = nullptr;
    funcs->AEIO_GetAuxChannelDesc     = nullptr;
    funcs->AEIO_FreeAuxChannel        = nullptr;
    funcs->AEIO_NumAuxFiles           = nullptr;
    funcs->AEIO_GetNthAuxFileSpec     = nullptr;
    funcs->AEIO_CloseSourceFiles      = nullptr;
    funcs->AEIO_CountUserData         = nullptr;
    funcs->AEIO_GetUserData           = nullptr;
    funcs->AEIO_VerifyFileImportable  = nullptr;
    funcs->AEIO_DrawAuxChannel        = nullptr;

    return A_Err_NONE;
}
}

namespace aegif
{
A_Err RegisterIO(AEGP_SuiteHandler& suites, AEGP_PluginID aegp_pluginId)
{
    AEGLOG_BLOCK("register AEIO module");

    AEIO_ModuleInfo moduleInfo;
    GUARD_A_Err(ConstructModuleInfo(suites, &moduleInfo));

    AEIO_FunctionBlock4 funcBlock;
    GUARD_A_Err(ConstructFunctionBlock(suites, &funcBlock));

    GUARD_A_Err(suites.RegisterSuite5()->AEGP_RegisterIO(aegp_pluginId, nullptr, &moduleInfo, &funcBlock));

    return A_Err_NONE;
}
}
