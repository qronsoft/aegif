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

A_Err AEIO_GetOutputInfo(AEIO_BasicData* basic_dataP, AEIO_OutSpecH outH, AEIO_Verbiage* verbiage)
{
    AEGLOG_BLOCK("AEIO_GetOutputInfo");
    // TODO: impl
    memset(verbiage->type, 0, sizeof(verbiage));
    strcpy(verbiage->type, "GIF");
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

A_Err AEIO_SetOutputFile(AEIO_BasicData* basic_dataP, AEIO_OutSpecH outH, const A_UTF16Char* file_pathZ)
{
    AEGLOG_BLOCK("AEIO_SetOutputFile");
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
    strcpy(info->name, "GIF Encoder");

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

    funcs->AEIO_InitOutputSpec    = aegif::CatchException<AEIO_InitOutputSpec>;
    funcs->AEIO_GetDepths         = aegif::CatchException<AEIO_GetDepths>;
    funcs->AEIO_GetOutputInfo     = aegif::CatchException<AEIO_GetOutputInfo>;
    funcs->AEIO_UserOptionsDialog = aegif::CatchException<AEIO_UserOptionsDialog>;
    funcs->AEIO_SetOutputFile     = aegif::CatchException<AEIO_SetOutputFile>;
    funcs->AEIO_StartAdding       = aegif::CatchException<AEIO_StartAdding>;
    funcs->AEIO_AddFrame          = aegif::CatchException<AEIO_AddFrame>;
    funcs->AEIO_EndAdding         = aegif::CatchException<AEIO_EndAdding>;

#define StubAEIOCallback(callbackName)                                                                       \
    [](auto...) -> A_Err {                                                                                   \
        AEGLOG_TRACE("stub: {} called", #callbackName);                                                      \
        return AEIO_Err_USE_DFLT_CALLBACK;                                                                   \
    }

    // stubs
    funcs->AEIO_InitInSpecFromFile     = StubAEIOCallback(AEIO_InitInSpecFromFile);
    funcs->AEIO_InitInSpecInteractive  = StubAEIOCallback(AEIO_InitInSpecInteractive);
    funcs->AEIO_DisposeInSpec          = StubAEIOCallback(AEIO_DisposeInSpec);
    funcs->AEIO_FlattenOptions         = StubAEIOCallback(AEIO_FlattenOptions);
    funcs->AEIO_InflateOptions         = StubAEIOCallback(AEIO_InflateOptions);
    funcs->AEIO_SynchInSpec            = StubAEIOCallback(AEIO_SynchInSpec);
    funcs->AEIO_GetActiveExtent        = StubAEIOCallback(AEIO_GetActiveExtent);
    funcs->AEIO_GetInSpecInfo          = StubAEIOCallback(AEIO_GetInSpecInfo);
    funcs->AEIO_DrawSparseFrame        = StubAEIOCallback(AEIO_DrawSparseFrame);
    funcs->AEIO_GetDimensions          = StubAEIOCallback(AEIO_GetDimensions);
    funcs->AEIO_GetDuration            = StubAEIOCallback(AEIO_GetDuration);
    funcs->AEIO_GetTime                = StubAEIOCallback(AEIO_GetTime);
    funcs->AEIO_GetSound               = StubAEIOCallback(AEIO_GetSound);
    funcs->AEIO_InqNextFrameTime       = StubAEIOCallback(AEIO_InqNextFrameTime);
    funcs->AEIO_GetFlatOutputOptions   = StubAEIOCallback(AEIO_GetFlatOutputOptions);
    funcs->AEIO_DisposeOutputOptions   = StubAEIOCallback(AEIO_DisposeOutputOptions);
    funcs->AEIO_OutputInfoChanged      = StubAEIOCallback(AEIO_OutputInfoChanged);
    funcs->AEIO_OutputFrame            = StubAEIOCallback(AEIO_OutputFrame);
    funcs->AEIO_WriteLabels            = StubAEIOCallback(AEIO_WriteLabels);
    funcs->AEIO_GetSizes               = StubAEIOCallback(AEIO_GetSizes);
    funcs->AEIO_Flush                  = StubAEIOCallback(AEIO_Flush);
    funcs->AEIO_AddSoundChunk          = StubAEIOCallback(AEIO_AddSoundChunk);
    funcs->AEIO_Idle                   = [](auto...) -> A_Err { return AEIO_Err_USE_DFLT_CALLBACK; };
    funcs->AEIO_GetOutputSuffix        = StubAEIOCallback(AEIO_GetOutputSuffix);
    funcs->AEIO_SeqOptionsDlg          = StubAEIOCallback(AEIO_SeqOptionsDlg);
    funcs->AEIO_GetNumAuxChannels      = StubAEIOCallback(AEIO_GetNumAuxChannels);
    funcs->AEIO_GetAuxChannelDesc      = StubAEIOCallback(AEIO_GetAuxChannelDesc);
    funcs->AEIO_DrawAuxChannel         = StubAEIOCallback(AEIO_DrawAuxChannel);
    funcs->AEIO_FreeAuxChannel         = StubAEIOCallback(AEIO_FreeAuxChannel);
    funcs->AEIO_NumAuxFiles            = StubAEIOCallback(AEIO_NumAuxFiles);
    funcs->AEIO_GetNthAuxFileSpec      = StubAEIOCallback(AEIO_GetNthAuxFileSpec);
    funcs->AEIO_CloseSourceFiles       = StubAEIOCallback(AEIO_CloseSourceFiles);
    funcs->AEIO_CountUserData          = StubAEIOCallback(AEIO_CountUserData);
    funcs->AEIO_SetUserData            = StubAEIOCallback(AEIO_SetUserData);
    funcs->AEIO_GetUserData            = StubAEIOCallback(AEIO_GetUserData);
    funcs->AEIO_AddMarker              = StubAEIOCallback(AEIO_AddMarker);
    funcs->AEIO_VerifyFileImportable   = StubAEIOCallback(AEIO_VerifyFileImportable);
    funcs->AEIO_UserAudioOptionsDialog = StubAEIOCallback(AEIO_UserAudioOptionsDialog);
    funcs->AEIO_AddMarker2             = StubAEIOCallback(AEIO_AddMarker2);
    funcs->AEIO_AddMarker3             = StubAEIOCallback(AEIO_AddMarker3);
    funcs->AEIO_GetMimeType            = StubAEIOCallback(AEIO_GetMimeType);

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
