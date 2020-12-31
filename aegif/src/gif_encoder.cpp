#include "gif_encoder.hpp"

// gifski
#include <gifski.h>

// aegif
#include "log.hpp"

namespace aegif
{
GIFEncoder::GIFEncoder(Options options, const std::string& outPathU8)
    : gifski_(nullptr)
    , error_(Error::NONE)
{
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
        error_ = Error::INVALID_OPTIONS;
    }

    if (gifski_set_file_output(gifski_, outPathU8.c_str()) != GifskiError::GIFSKI_OK)
    {
        AEGLOG_ERROR("invalid output destination");
        error_ = Error::INVALID_OUTPUT_DESTINATION;
    }
}
GIFEncoder::~GIFEncoder()
{
    if (gifski_)
    {
        gifski_finish(gifski_);
    }
}
GIFEncoder::Error GIFEncoder::AddFrameARGB(
    uint32_t frameIdx,
    uint32_t width,
    uint32_t height,
    uint32_t bytesPerRow,
    const unsigned char* pixels,
    double displayTimeSec)
{
    if (error_ != Error::NONE) return error_;

    if (gifski_add_frame_argb(gifski_, frameIdx, width, bytesPerRow, height, pixels, displayTimeSec)
        != GIFSKI_OK)
    {
        return error_ = Error::ENCODE_FAILED;
    }

    return Error::NONE;
}
}
