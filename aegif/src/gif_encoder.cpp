#include "gif_encoder.hpp"

// gifski
#include <gifski.h>

// aegif
#include "log.hpp"

namespace aegif
{
GIFEncoder::GIFEncoder()
    : gifski_(nullptr)
{
}
GIFEncoder::~GIFEncoder()
{
    Finish();
}
GIFEncoder::Error GIFEncoder::Init(Options options)
{
    if (gifski_ != nullptr)
    {
        return Error::ALREADY_INITED;
    }

    GifskiSettings gifskiSettings = {};

    gifskiSettings.width      = options.width;
    gifskiSettings.height     = options.height;
    gifskiSettings.quality    = options.quality;
    gifskiSettings.loop_count = options.loopCnt;
    gifskiSettings.fast       = options.fast;

    gifski_ = gifski_new(&gifskiSettings);
    if (gifski_ == nullptr)
    {
        AEGLOG_ERROR("invalid encode options was passed");
        return Error::INVALID_OPTIONS;
    }

    return Error::NONE;
}
GIFEncoder::Error GIFEncoder::SetOutputPath(const std::string& outPathU8)
{
    if (gifski_ == nullptr) return Error::NOT_INITED;

    if (gifski_set_file_output(gifski_, outPathU8.c_str()) != GifskiError::GIFSKI_OK)
    {
        // TODO: detailed error reporting
        AEGLOG_ERROR("invalid output destination");
        return Error::INVALID_OUTPUT_DESTINATION;
    }
    return Error::NONE;
}
GIFEncoder::Error GIFEncoder::AddFrameARGB(
    uint32_t frameIdx,
    uint32_t width,
    uint32_t height,
    uint32_t bytesPerRow,
    const unsigned char* pixels,
    double displayTimeSec)
{
    if (gifski_ == nullptr) return Error::NOT_INITED;

    if (gifski_add_frame_argb(gifski_, frameIdx, width, bytesPerRow, height, pixels, displayTimeSec)
        != GIFSKI_OK)
    {
        return Error::ENCODE_FAILED;
    }

    return Error::NONE;
}
GIFEncoder::Error GIFEncoder::Finish()
{
    if (gifski_ == nullptr) return Error::NOT_INITED;

    const auto err = gifski_finish(gifski_);
    gifski_        = nullptr;

    if (err != GifskiError::GIFSKI_OK)
    {
        // TODO: detailed error reporting
        return Error::ENCODE_FAILED;
    }

    return Error::NONE;
}
}
