#include "output_options_v255.hpp"

// std
#include <algorithm>

// mpack
#include <mpack.h>

// aegif
#include "log.hpp"
#include "scope_guard.hpp"

namespace aegif
{
namespace frozen
{
OutputOptionsV255::OutputOptionsV255() = default;

bool OutputOptionsV255::Deserialize(const char* bytes, size_t size)
{
    if (bytes == nullptr) return false;

    Data newData;

    mpack_tree_t tree;
    mpack_tree_init_data(&tree, bytes, size);
    mpack_tree_parse(&tree);
    auto root = mpack_tree_root(&tree);

#define load_msgpack_data(name, type)                                      \
    do                                                                     \
    {                                                                      \
        auto entry = mpack_node_map_uint_optional(root, Data::ID_##name);  \
        if (mpack_node_type(entry) == mpack_type_##type)                   \
        {                                                                  \
            const auto value = mpack_node_##type(entry);                   \
            if (mpack_node_error(entry) == mpack_ok)                       \
            {                                                              \
                newData.name = static_cast<decltype(newData.name)>(value); \
            }                                                              \
        }                                                                  \
    } while (0);

    load_msgpack_data(version, uint);
    load_msgpack_data(loop, bool);
    // load_msgpack_data(fast, bool);
    load_msgpack_data(quality, uint);
    load_msgpack_data(encoderP, uint);
    load_msgpack_data(foo_s, str);
    load_msgpack_data(foo_u32, uint);

    if (mpack_tree_destroy(&tree) != mpack_ok)
    {
        AEGLOG_ERROR("failed to deserialize output options");
        return false;
    }

    newData.version = CURRENT_VERSION;
    newData.quality = std::clamp(newData.quality, QUALITY_MIN, QUALITY_MAX);

    data_ = newData;

    return true;
}

bool OutputOptionsV255::Deserialize(const std::vector<char>& bytes)
{
    return Deserialize(bytes.data(), bytes.size());
}

bool OutputOptionsV255::Serialize(std::vector<char>* bytes) const
{
    if (bytes == nullptr) return false;
    bytes->clear();

    char* ptr   = nullptr;
    size_t size = 0;

    ScopeGuard freeMem([&]() { free(ptr); });

    mpack_writer_t writer;
    mpack_writer_init_growable(&writer, &ptr, &size);

#define store_msgpack_data(name, type)            \
    do                                            \
    {                                             \
        mpack_write_u8(&writer, Data::ID_##name); \
        mpack_write_##type(&writer, data_.name);  \
    } while (0);

    mpack_start_map(&writer, Data::COUNT);
    store_msgpack_data(version, u8);
    store_msgpack_data(loop, bool);
    // store_msgpack_data(fast, bool);
    store_msgpack_data(quality, u8);
    store_msgpack_data(encoderP, u64);

    mpack_write_u8(&writer, Data::ID_foo_s);
    mpack_write_cstr(&writer, data_.foo_s.c_str());

    mpack_write_u8(&writer, Data::ID_foo_u32);
    mpack_write_u32(&writer, data_.foo_u32);

    mpack_finish_map(&writer);

    if (mpack_writer_destroy(&writer) != mpack_ok)
    {
        AEGLOG_ERROR("failed to serialize output options");
        return false;
    }

    bytes->assign(ptr, ptr + size);
    return true;
}
void OutputOptionsV255::quality(uint8_t x)
{
    data_.quality = std::clamp(x, QUALITY_MIN, QUALITY_MAX);
}

bool OutputOptionsV255::operator==(const OutputOptionsV255& other) const
{
    if (version() != other.version()) return false;
    if (loop() != other.loop()) return false;
    // if (fast() != other.fast()) return false;
    if (quality() != other.quality()) return false;
    if (encoderP() != other.encoderP()) return false;
    if (data_.foo_u32 != other.data_.foo_u32) return false;
    if (data_.foo_s != other.data_.foo_s) return false;
    return true;
}
bool OutputOptionsV255::operator!=(const OutputOptionsV255& other) const
{
    return !(*this == other);
}
}
}
