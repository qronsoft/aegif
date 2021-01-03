#pragma once

// std
#include <cstdint>
#include <string>

struct gifski;

namespace aegif
{
class GIFEncoder
{
public:
    struct Options
    {
        uint32_t width  = 0;
        uint32_t height = 0;
        bool loop       = true;
        bool fast       = false;
        uint8_t quality = 100; /* 1 ~ 100 */
    };

    GIFEncoder();
    ~GIFEncoder();

    enum class Error
    {
        NONE = 0,
        NOT_INITED,
        INVALID_OPTIONS,
        INVALID_OUTPUT_DESTINATION,
        ENCODE_FAILED,
        ALREADY_INITED
    };

    Error Init(Options options);

    Error SetOutputPath(const std::string& outPathU8);

    Error AddFrameARGB(
        uint32_t frameIdx,
        uint32_t width,
        uint32_t height,
        uint32_t bytesPerRow,
        const unsigned char* pixels,
        double displayTimeSec);

    Error Finish();

private:
    GIFEncoder(const GIFEncoder&) = delete;
    GIFEncoder& operator=(const GIFEncoder&) = delete;

    struct gifski* gifski_;
};
}
