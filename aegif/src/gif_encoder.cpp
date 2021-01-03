#include "gif_encoder.hpp"

// std
#include <algorithm>
#include <future>

// gifski
#include <gifski.h>

// aegif
#include "log.hpp"

namespace
{
template <class Fn>
void ParallelFor(int loopCnt, Fn func)
{
    int coreCnt = std::max((int) std::thread::hardware_concurrency(), 1);
    std::vector<std::future<void>> tasks(coreCnt);
    for (int coreId = 0; coreId < coreCnt; ++coreId)
    {
        tasks[coreId] = std::async(
            std::launch::async | std::launch::deferred,
            [&](int id) {
                const int begin = loopCnt / coreCnt * id + std::min(loopCnt % coreCnt, id);
                const int end   = loopCnt / coreCnt * (id + 1) + std::min(loopCnt % coreCnt, id + 1);
                for (int i = begin; i < end; ++i)
                {
                    func(i);
                }
            },
            coreId);
    }
    for (auto&& task : tasks)
    {
        task.wait();
    }
}
}

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
    gifskiSettings.quality    = std::clamp(options.quality, uint8_t(1), uint8_t(100));
    gifskiSettings.loop_count = options.loop ? 0 : -1;
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

    ConvertARGBToRGB(width, height, bytesPerRow, pixels);

    if (gifski_add_frame_rgb(
            gifski_,
            frameIdx,
            width,
            rgbPixelBuf_.size() / height,
            height,
            rgbPixelBuf_.data(),
            displayTimeSec)
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
void GIFEncoder::ConvertARGBToRGB(
    uint32_t width, uint32_t height, uint32_t bytesPerRow, const unsigned char* srcPixels)
{
    rgbPixelBuf_.resize(width * height * 3, 0);
    unsigned char* dstPixels = rgbPixelBuf_.data();

    ParallelFor(height, [&](int y) {
        const unsigned char* srcRow = srcPixels + y * bytesPerRow;
        unsigned char* dstRow       = dstPixels + y * width * 3;
        for (int x = 0; x < width; ++x)
        {
            const unsigned char* argb = srcRow + (x << 2);
            const auto a              = uint16_t(argb[0]);
            const auto r              = uint16_t(argb[1]);
            const auto g              = uint16_t(argb[2]);
            const auto b              = uint16_t(argb[3]);
            unsigned char* rgb        = dstRow + x * 3;
            *(rgb + 0)                = unsigned char(r * a / 255);
            *(rgb + 1)                = unsigned char(g * a / 255);
            *(rgb + 2)                = unsigned char(b * a / 255);
        }
    });
}
}
