#pragma once

// std
#include <cstdint>
#include <vector>

// msgpack
#include <msgpack.hpp>

namespace aegif
{
class GIFEncoder;
}

namespace aegif
{
class OutputOptions
{
public:
    OutputOptions();
    bool Load(const char* bytes, size_t size);
    bool Load(const std::vector<char>& bytes);
    bool Serialize(std::vector<char>* bytes) const;

    uint8_t version() const { return data_.version; }
    bool loop() const { return data_.loop; }
    void loop(bool x) { data_.loop = x; }
    bool fast() const { return data_.fast; }
    void fast(bool x) { data_.fast = x; }
    uint8_t quality() const { return data_.quality; }
    void quality(uint8_t x) { data_.quality = x; }
    GIFEncoder* encoderP() const { return (GIFEncoder*) (data_.encoderP); }
    void encoderP(GIFEncoder* x) { data_.encoderP = uint64_t(x); }

private:
    struct Data
    {
        /* data */
        // dont change member name
        uint8_t version = 1;
        // v1
        bool loop         = true;
        bool fast         = false;
        uint8_t quality   = 100; /* 1 ~ 100 */
        uint64_t encoderP = 0;
        // end v1

        MSGPACK_DEFINE_MAP(version, loop, fast, quality, encoderP);
    } data_;
};
}
