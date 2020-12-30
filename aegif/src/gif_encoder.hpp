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
        uint8_t quality = 100; /* 1 ~ 100*/
        bool once       = false;
        bool fast       = true;
    };

    GIFEncoder(Options options, const std::string& outPathU8);
    ~GIFEncoder();

    enum class Error
    {
        NONE = 0,
        INVALID_OPTIONS,
        INVALID_OUTPUT_DESTINATION,
        ENCODE_FAILED
    };
    Error GetError() const { return error_; }
    bool HasError() const { return GetError() != Error::NONE; }

    Error AddFrameARGB(
        uint32_t frameIdx,
        uint32_t width,
        uint32_t height,
        uint32_t bytesPerRow,
        const unsigned char* pixels,
        double displayTimeSec);
private:
    GIFEncoder(const GIFEncoder&) = delete;
    GIFEncoder& operator=(const GIFEncoder&) = delete;

    struct gifski* gifski_;
    Error error_;
};
}
