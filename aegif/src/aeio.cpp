#include "aeio.hpp"

// std
#include <memory>
#include <cstdint>

// AE
#include <AE_Macros.h>

// spdlog
#include <spdlog/spdlog.h>
#include <spdlog/fmt/bin_to_hex.h>

// aegif
#include "log.hpp"
#include "charenc.hpp"
#include "error_guard.hpp"
#include "scope_guard.hpp"
#include "catch_exception.hpp"
#include "aegp_mem.hpp"
#include "gif_encoder.hpp"
#include "output_options.hpp"
#include "options_dialog.hpp"

namespace
{
A_Err ReportError(AEIO_BasicData* basic_dataP, const A_char* msg, A_Err err)
{
    AEGLOG_ERROR(msg);
    basic_dataP->msg_func(err, msg);
    return err;
}

A_Err GetMemHOptionsData(
    const SPBasicSuite* pica_basicP, AEGP_MemHandle memH, std::unique_ptr<aegif::OutputOptions>* options)
{
    GUARD_ERROR(options != nullptr, A_Err_GENERIC);
    options->reset();
    if (memH == nullptr) return A_Err_NONE;

    std::vector<char> serialized;
    GUARD_A_Err(aegif::GetMemHData(pica_basicP, memH, &serialized));

    std::unique_ptr<aegif::OutputOptions> opt = std::make_unique<aegif::OutputOptions>();
    if (opt->Load(serialized))
    {
        *options = std::move(opt);
    }
    else
    {
        AEGLOG_WARN("invalid options binary: {:X}", spdlog::to_hex(serialized));
    }

    return A_Err_NONE;
}

A_Err SetMemHOptionsData(
    const SPBasicSuite* pica_basicP, AEGP_MemHandle memH, const aegif::OutputOptions& options)
{
    if (memH == nullptr) return A_Err_NONE;

    std::vector<char> serialized;
    if (options.Serialize(&serialized))
    {
        GUARD_A_Err(aegif::SetMemHData(pica_basicP, memH, serialized));
    }
    else
    {
        AEGLOG_ERROR("failed to serialized options data");
        return A_Err_GENERIC;
    }

    return A_Err_NONE;
}

A_Err GetOutSpecOptionsData(
    const SPBasicSuite* pica_basicP, AEIO_OutSpecH outH, std::unique_ptr<aegif::OutputOptions>* options)
{
    GUARD_ERROR(options != nullptr, A_Err_GENERIC);
    options->reset();

    AEGP_SuiteHandler suites(pica_basicP);
    AEIO_Handle optionsH = nullptr;
    GUARD_A_Err(
        suites.IOOutSuite4()->AEGP_GetOutSpecOptionsHandle(outH, reinterpret_cast<void**>(&optionsH)));
    GUARD_A_Err(GetMemHOptionsData(pica_basicP, optionsH, options));
    return A_Err_NONE;
}

A_Err UpdateOutSpecOptionsData(
    const SPBasicSuite* pica_basicP, AEIO_OutSpecH outH, const aegif::OutputOptions& options)
{
    AEGP_SuiteHandler suites(pica_basicP);
    AEIO_Handle optionsH = nullptr;
    GUARD_A_Err(
        suites.IOOutSuite4()->AEGP_GetOutSpecOptionsHandle(outH, reinterpret_cast<void**>(&optionsH)));
    GUARD_A_Err(SetMemHOptionsData(pica_basicP, optionsH, options));
    return A_Err_NONE;
}

A_Err GetFilePathU8FromOutSpecH(AEIO_BasicData* basic_dataP, AEIO_OutSpecH outH, std::string* filePathU8)
{
    AEGP_SuiteHandler suites(basic_dataP->pica_basicP);
    A_Boolean fileReserved;
    AEGP_MemHandle filePathH = nullptr;
    GUARD_A_Err(suites.IOOutSuite4()->AEGP_GetOutSpecFilePath(outH, &filePathH, &fileReserved));
    aegif::ScopeGuard freeFilePathH([&]() {
        if (filePathH)
        {
            suites.MemorySuite1()->AEGP_FreeMemHandle(filePathH);
        }
    });

    if (filePathH == nullptr)
    {
        filePathU8->clear();
    }
    else
    {
        std::basic_string<A_UTF16Char> filePathU16;
        GUARD_A_Err(aegif::GetMemHString(basic_dataP->pica_basicP, filePathH, &filePathU16));
        GUARD_A_Err(suites.MemorySuite1()->AEGP_FreeMemHandle(filePathH));
        filePathH   = nullptr;
        *filePathU8 = aegif::U16ToU8(filePathU16);
    }
    return A_Err_NONE;
}
}

/* callbacks */
namespace
{
A_Err AEIO_InitOutputSpec(AEIO_BasicData* basic_dataP, AEIO_OutSpecH outH, A_Boolean* user_interacted)
{
    AEGLOG_BLOCK("AEIO_InitOutputSpec");

    AEGP_SuiteHandler suites(basic_dataP->pica_basicP);

    *user_interacted = FALSE;

    // create new options
    AEIO_Handle newOptionsH = nullptr;
    aegif::ScopeGuard freeNewOptionsH([&]() {
        if (newOptionsH)
        {
            suites.MemorySuite1()->AEGP_FreeMemHandle(newOptionsH);
        }
    });

    GUARD_A_Err(suites.MemorySuite1()->AEGP_NewMemHandle(
        basic_dataP->aegp_plug_id, "InitOutputSpec options", 1, AEGP_MemFlag_CLEAR, &newOptionsH));
    GUARD_ERROR(newOptionsH != nullptr, A_Err_ALLOC);

    {
        std::unique_ptr<aegif::OutputOptions> oldOptions;
        GUARD_A_Err(GetOutSpecOptionsData(basic_dataP->pica_basicP, outH, &oldOptions));

        if (oldOptions)
        {
            AEGLOG_TRACE("inherit previous options data");
            SetMemHOptionsData(basic_dataP->pica_basicP, newOptionsH, *oldOptions);
        }
        else
        {
            AEGLOG_TRACE("create new options data");
            SetMemHOptionsData(basic_dataP->pica_basicP, newOptionsH, aegif::OutputOptions());
        }
    }

    AEIO_Handle oldOptionsH = nullptr;
    GUARD_A_Err(suites.IOOutSuite4()->AEGP_SetOutSpecOptionsHandle(
        outH, newOptionsH, reinterpret_cast<void**>(&oldOptionsH)));

    newOptionsH = nullptr;

    if (oldOptionsH)
    {
        GUARD_A_Err(suites.MemorySuite1()->AEGP_FreeMemHandle(oldOptionsH));
    }

    return A_Err_NONE;
}

A_Err AEIO_GetFlatOutputOptions(AEIO_BasicData* basic_dataP, AEIO_OutSpecH outH, AEIO_Handle* newOptionsPH)
{
    AEGLOG_BLOCK("AEIO_GetFlatOutputOptions");

    *newOptionsPH = nullptr;

    AEGP_SuiteHandler suites(basic_dataP->pica_basicP);

    // get old options
    AEIO_Handle oldOptionsH = nullptr;
    GUARD_A_Err(
        suites.IOOutSuite4()->AEGP_GetOutSpecOptionsHandle(outH, reinterpret_cast<void**>(&oldOptionsH)));
    if (oldOptionsH == nullptr)
    {
        return A_Err_NONE;
    }

    // create new options
    AEGP_MemHandle newOptionsH = nullptr;
    aegif::ScopeGuard freeNewOptionsH([&]() {
        if (*newOptionsPH == nullptr && newOptionsH != nullptr)
        {
            suites.MemorySuite1()->AEGP_FreeMemHandle(newOptionsH);
        }
    });
    GUARD_A_Err(suites.MemorySuite1()->AEGP_NewMemHandle(
        basic_dataP->aegp_plug_id, "flat optionsH", 1, AEGP_MemFlag_CLEAR, &newOptionsH));
    GUARD_ERROR(newOptionsH != nullptr, A_Err_ALLOC);

    // copy options
    std::unique_ptr<aegif::OutputOptions> oldOptions;
    GUARD_A_Err(GetMemHOptionsData(basic_dataP->pica_basicP, oldOptionsH, &oldOptions));
    GUARD_A_Err(SetMemHOptionsData(
        basic_dataP->pica_basicP, newOptionsH, oldOptions ? *oldOptions : aegif::OutputOptions()));

    *newOptionsPH = newOptionsH;

    return A_Err_NONE;
}

A_Err AEIO_GetDepths(AEIO_BasicData* basic_dataP, AEIO_OutSpecH outH, AEIO_SupportedDepthFlags* which)
{
    AEGLOG_BLOCK("AEIO_GetDepths");
    *which = AEIO_SupportedDepthFlags_NONE;
    *which |= AEIO_SupportedDepthFlags_DEPTH_24;
    return A_Err_NONE;
}

A_Err AEIO_DisposeOutputOptions(AEIO_BasicData* basic_dataP, void* optionsPV)
{
    AEGLOG_BLOCK("AEIO_DisposeOutputOptions");
    AEGP_SuiteHandler suites(basic_dataP->pica_basicP);
    if (auto optionsH = reinterpret_cast<AEIO_Handle>(optionsPV))
    {
        GUARD_A_Err(suites.MemorySuite1()->AEGP_FreeMemHandle(optionsH));
    }

    return A_Err_NONE;
};

A_Err AEIO_UserOptionsDialog(
    AEIO_BasicData* basic_dataP,
    AEIO_OutSpecH outH,
    const PF_EffectWorld* sample0,
    A_Boolean* user_interacted0)
{
    AEGLOG_BLOCK("AEIO_UserOptionsDialog");
    *user_interacted0 = FALSE;

    static bool s_isOptionsDialogOpened = false;
    if (s_isOptionsDialogOpened)
    {
        AEGLOG_TRACE("options dialog is now already open");
        return A_Err_NONE;
    }
    aegif::ScopeGuard onExit([]() { s_isOptionsDialogOpened = false; });
    s_isOptionsDialogOpened = true;

    AEGP_SuiteHandler suites(basic_dataP->pica_basicP);

    void* mainWindowHandle = nullptr;
    GUARD_A_Err(suites.UtilitySuite3()->AEGP_GetMainHWND(&mainWindowHandle));

    std::unique_ptr<aegif::OutputOptions> options;
    GUARD_A_Err(GetOutSpecOptionsData(basic_dataP->pica_basicP, outH, &options));
    GUARD_ERROR(options != nullptr, A_Err_STRUCT);

    aegif::OptionsDialog optionsDialog(mainWindowHandle);
    optionsDialog.SetLocale(aegif::GetAppLocale(basic_dataP->pica_basicP));
    optionsDialog.SetLoopEnable(options->loop());
    optionsDialog.SetFastEncodeEnable(options->fast());
    optionsDialog.SetImageQuality(options->quality());

    if (!optionsDialog.ShowModal())
    {
        AEGLOG_TRACE("dialog input discarded");
        return A_Err_NONE;
    }

    AEGLOG_TRACE("dialog input accepted");

    options->loop(optionsDialog.GetLoopEnable());
    options->fast(optionsDialog.GetFastEncodeEnable());
    options->quality(optionsDialog.GetImageQuality());

    GUARD_A_Err(UpdateOutSpecOptionsData(basic_dataP->pica_basicP, outH, *options));

    AEGLOG_INFO("options has been changed");
    AEGLOG_INFO("loop: {}", options->loop());
    AEGLOG_INFO("fast: {}", options->fast());
    AEGLOG_INFO("quality: {}", options->quality());

    *user_interacted0 = TRUE;
    return A_Err_NONE;
}

A_Err AEIO_StartAdding(AEIO_BasicData* basic_dataP, AEIO_OutSpecH outH, A_long flags)
{
    AEGLOG_BLOCK("AEIO_StartAdding");
    AEGP_SuiteHandler suites(basic_dataP->pica_basicP);

    std::unique_ptr<aegif::OutputOptions> options;
    GUARD_A_Err(GetOutSpecOptionsData(basic_dataP->pica_basicP, outH, &options));
    GUARD_ERROR(options != nullptr, A_Err_STRUCT);

    A_long width  = 0;
    A_long height = 0;
    GUARD_A_Err(suites.IOOutSuite4()->AEGP_GetOutSpecDimensions(outH, &width, &height));

    std::string filePathU8;
    GUARD_A_Err(GetFilePathU8FromOutSpecH(basic_dataP, outH, &filePathU8));
    GUARD_ERROR(!filePathU8.empty(), AEIO_Err_BAD_FILENAME);

    aegif::GIFEncoder::Options encoderOptions;
    encoderOptions.width   = width;
    encoderOptions.height  = height;
    encoderOptions.loop    = options->loop();
    encoderOptions.fast    = options->fast();
    encoderOptions.quality = options->quality();
    AEGLOG_INFO("encode options");
    AEGLOG_INFO("width: {}", encoderOptions.width);
    AEGLOG_INFO("height: {}", encoderOptions.height);
    AEGLOG_INFO("loop: {}", encoderOptions.loop);
    AEGLOG_INFO("fast: {}", encoderOptions.fast);
    AEGLOG_INFO("quality: {}", encoderOptions.quality);

    aegif::GIFEncoder* encoder = nullptr;
    aegif::ScopeGuard deleteEncoder([&]() {
        if (encoder)
        {
            delete encoder;
        }
    });
    encoder = new aegif::GIFEncoder();

    if (encoder->Init(encoderOptions) != aegif::GIFEncoder::Error::NONE)
    {
        return ReportError(basic_dataP, "invalid encode options passed", A_Err_GENERIC);
    }

    if (encoder->SetOutputPath(filePathU8) != aegif::GIFEncoder::Error::NONE)
    {
        return ReportError(basic_dataP, "invalid output destination", A_Err_GENERIC);
    }

    options->encoderP(encoder);
    GUARD_A_Err(UpdateOutSpecOptionsData(basic_dataP->pica_basicP, outH, *options));
    encoder = nullptr;

    AEGLOG_INFO("encode started!");

    return A_Err_NONE;
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
    AEGLOG_INFO("frame: {}", frame_index);

    GUARD_ERROR(PF_WORLD_IS_DEEP(wP) == 0, AEIO_Err_BAD_BMWORLD);

    AEGP_SuiteHandler suites(basic_dataP->pica_basicP);

    std::unique_ptr<aegif::OutputOptions> options;
    GUARD_A_Err(GetOutSpecOptionsData(basic_dataP->pica_basicP, outH, &options));
    GUARD_ERROR(options != nullptr, A_Err_STRUCT);

    GUARD_ERROR(options->encoderP() != nullptr, A_Err_GENERIC);
    auto encoder = options->encoderP();

    A_Fixed fps = 0;
    GUARD_A_Err(suites.IOOutSuite4()->AEGP_GetOutSpecFPS(outH, &fps));
    const double timeSec = 1.0 * frame_index / FIX_2_FLOAT(fps);

    if (encoder->AddFrameARGB(
            frame_index, wP->width, wP->height, wP->rowbytes, (const unsigned char*) wP->data, timeSec)
        != aegif::GIFEncoder::Error::NONE)
    {
        return ReportError(basic_dataP, "failed to add frame to gif encoder", A_Err_GENERIC);
    }

    return A_Err_NONE;
}

A_Err AEIO_EndAdding(AEIO_BasicData* basic_dataP, AEIO_OutSpecH outH, A_long flags)
{
    AEGLOG_BLOCK("AEIO_EndAdding");

    std::unique_ptr<aegif::OutputOptions> options;
    GUARD_A_Err(GetOutSpecOptionsData(basic_dataP->pica_basicP, outH, &options));
    GUARD_ERROR(options != nullptr, A_Err_STRUCT);

    std::unique_ptr<aegif::GIFEncoder> encoder(options->encoderP());
    options->encoderP(nullptr);
    GUARD_A_Err(UpdateOutSpecOptionsData(basic_dataP->pica_basicP, outH, *options));

    if (encoder->Finish() != aegif::GIFEncoder::Error::NONE)
    {
        return ReportError(basic_dataP, "encoding did not complete successfully", A_Err_GENERIC);
    }

    AEGLOG_INFO("encode completed!");

    return A_Err_NONE;
}

A_Err ConstructModuleInfo(AEGP_SuiteHandler& suites, AEIO_ModuleInfo* info)
{
    AEGLOG_BLOCK("ConstructModuleInfo");

    AEFX_CLR_STRUCT(*info);

    info->sig = 'qAEG';
    strcpy(info->name, "AeGIF");

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

#define default_cb(callbackName)                                                                             \
    [](auto...) -> A_Err {                                                                                   \
        AEGLOG_DEBUG("default {} called", #callbackName);                                                    \
        return AEIO_Err_USE_DFLT_CALLBACK;                                                                   \
    }

#define stub_cb(callbackName)                                                                                \
    [](auto...) -> A_Err {                                                                                   \
        AEGLOG_DEBUG("stub {} called", #callbackName);                                                       \
        return A_Err_NONE;                                                                                   \
    }

    /* required */
    funcs->AEIO_InitOutputSpec       = aegif::CatchException<AEIO_InitOutputSpec>;
    funcs->AEIO_GetFlatOutputOptions = aegif::CatchException<AEIO_GetFlatOutputOptions>;
    funcs->AEIO_SetOutputFile        = default_cb(AEIO_SetOutputFile);
    funcs->AEIO_StartAdding          = aegif::CatchException<AEIO_StartAdding>;
    funcs->AEIO_AddFrame             = aegif::CatchException<AEIO_AddFrame>;
    funcs->AEIO_EndAdding            = aegif::CatchException<AEIO_EndAdding>;
    funcs->AEIO_WriteLabels          = default_cb(AEIO_WriteLabels);
    funcs->AEIO_GetSizes             = default_cb(AEIO_GetSizes);
    funcs->AEIO_GetDepths            = aegif::CatchException<AEIO_GetDepths>;
    funcs->AEIO_GetOutputSuffix      = default_cb(AEIO_GetOutputSuffix);

    /* not required */
    funcs->AEIO_DisposeOutputOptions = aegif::CatchException<AEIO_DisposeOutputOptions>;
    funcs->AEIO_UserOptionsDialog    = aegif::CatchException<AEIO_UserOptionsDialog>;
    funcs->AEIO_GetOutputInfo        = stub_cb(AEIO_GetOutputInfo);
    funcs->AEIO_OutputInfoChanged    = stub_cb(AEIO_OutputInfoChanged);
    funcs->AEIO_Flush                = stub_cb(AEIO_Flush);
    funcs->AEIO_Idle                 = nullptr;
    funcs->AEIO_SetUserData          = stub_cb(AEIO_SetUserData);
    funcs->AEIO_GetMimeType          = stub_cb(AEIO_GetMimeType);

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
