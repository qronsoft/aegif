#include <gtest/gtest.h>

#include "output_options.hpp"
#include "frozen/output_options_V1.hpp"
#include "frozen/output_options_V255.hpp"

using namespace aegif;

TEST(OutputOptions, default_value)
{
    OutputOptions options;
    EXPECT_EQ(options.loop(), true);
    EXPECT_EQ(options.fast(), false);
    EXPECT_EQ(options.quality(), 100);
    EXPECT_EQ(options.encoderP(), nullptr);
}

TEST(OutputOptions, deserialize)
{
    OutputOptions options1;
    options1.loop(false);
    options1.fast(true);
    options1.quality(50);
    options1.encoderP((aegif::GIFEncoder*) size_t(0x12345678));

    std::vector<char> serialized;
    EXPECT_TRUE(options1.Serialize(&serialized));

    OutputOptions options2;
    EXPECT_TRUE(options2.Deserialize(serialized));

    EXPECT_EQ(options1, options2);
}

TEST(OutputOptions, serialize_fail_for_nullptr)
{
    OutputOptions options;
    EXPECT_FALSE(options.Serialize(nullptr));
}

TEST(OutputOptions, deserialize_fail_for_empty_data)
{
    OutputOptions options;
    EXPECT_FALSE(options.Deserialize(nullptr, 0));
    EXPECT_FALSE(options.Deserialize(nullptr, 100));
    EXPECT_FALSE(options.Deserialize(std::vector<char>()));
}

TEST(OutputOptions, deserialize_fail_for_invalid_data)
{
    OutputOptions options;
    std::vector<char> serialized1(0xff, 20);
    EXPECT_FALSE(options.Deserialize(serialized1));
}

TEST(OutputOptions, clamp_quality)
{
    OutputOptions options;
    options.quality(options.QUALITY_MIN - 1);
    EXPECT_EQ(options.quality(), options.QUALITY_MIN);
    options.quality(options.QUALITY_MAX + 1);
    EXPECT_EQ(options.quality(), options.QUALITY_MAX);
}

frozen::OutputOptionsV1 toV1(const OutputOptions& options)
{
    frozen::OutputOptionsV1 v1;
    v1.loop(options.loop());
    v1.fast(options.fast());
    v1.quality(options.quality());
    v1.encoderP(options.encoderP());
    return v1;
}

TEST(OutputOptions, v1_to_vx)
{
    frozen::OutputOptionsV1 v1;
    std::vector<char> serialized;
    EXPECT_TRUE(v1.Serialize(&serialized));

    OutputOptions vx;
    EXPECT_TRUE(vx.Deserialize(serialized));
    EXPECT_EQ(toV1(vx), v1);
}

TEST(OutputOptions, vx_to_v1)
{
    OutputOptions vx;
    std::vector<char> serialized;
    EXPECT_TRUE(vx.Serialize(&serialized));

    frozen::OutputOptionsV1 v1;
    EXPECT_TRUE(v1.Deserialize(serialized));
    EXPECT_EQ(v1, toV1(vx));
}

frozen::OutputOptionsV255 toV255(const OutputOptions& options)
{
    frozen::OutputOptionsV255 V255;
    V255.loop(options.loop());
    V255.quality(options.quality());
    V255.encoderP(options.encoderP());
    return V255;
}

TEST(OutputOptions, v255_to_Vx)
{
    frozen::OutputOptionsV255 v255;
    std::vector<char> serialized;
    EXPECT_TRUE(v255.Serialize(&serialized));

    OutputOptions vx;
    EXPECT_TRUE(vx.Deserialize(serialized));
    EXPECT_EQ(toV255(vx), v255);
}

TEST(OutputOptions, vx_to_v255)
{
    OutputOptions vx;
    std::vector<char> serialized;
    EXPECT_TRUE(vx.Serialize(&serialized));

    frozen::OutputOptionsV255 v255;
    EXPECT_TRUE(v255.Deserialize(serialized));
    EXPECT_EQ(v255, toV255(vx));
}
