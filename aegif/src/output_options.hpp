#pragma once

// std
#include <cstdint>
#include <vector>

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
        uint8_t version = 1;
        // v1
        bool loop         = true;
        bool fast         = false;
        uint8_t quality   = 100; /* 1 ~ 100 */
        uint64_t encoderP = 0;
        // end v1

        static constexpr uint8_t ID_version  = 0;
        static constexpr uint8_t ID_loop     = 1;
        static constexpr uint8_t ID_fast     = 2;
        static constexpr uint8_t ID_quality  = 3;
        static constexpr uint8_t ID_encoderP = 4;
        static constexpr uint8_t COUNT       = 5;
    } data_;
};
}
