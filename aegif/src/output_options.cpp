#include "output_options.hpp"

// std
#include <sstream>

// mpack
#include <mpack.h>

// aegif
#include "log.hpp"
#include "scope_guard.hpp"

namespace aegif
{
OutputOptions::OutputOptions() = default;

bool OutputOptions::Load(const char* bytes, size_t size)
{
    Data newData;

    mpack_tree_t tree;
    mpack_tree_init_data(&tree, bytes, size);
    mpack_tree_parse(&tree);
    auto root = mpack_tree_root(&tree);

#define load_msgpack_data(name, type)                                  \
    do                                                                 \
    {                                                                  \
        auto entry       = mpack_node_map_uint(root, Data::ID_##name); \
        const auto value = mpack_node_##type(entry);                   \
        if (mpack_node_error(entry) == mpack_ok)                       \
        {                                                              \
            newData.name = static_cast<decltype(newData.name)>(value); \
        }                                                              \
    } while (0);

    load_msgpack_data(version, u8);
    load_msgpack_data(loop, bool);
    load_msgpack_data(fast, bool);
    load_msgpack_data(quality, u8);
    load_msgpack_data(encoderP, u64);

    if (mpack_tree_destroy(&tree) != mpack_ok)
    {
        AEGLOG_ERROR("failed to load output options");
        return false;
    }

    data_ = newData;

    return true;
}

bool OutputOptions::Load(const std::vector<char>& bytes)
{
    return Load(bytes.data(), bytes.size());
}

bool OutputOptions::Serialize(std::vector<char>* bytes) const
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
    store_msgpack_data(fast, bool);
    store_msgpack_data(quality, u8);
    store_msgpack_data(encoderP, u64);
    mpack_finish_map(&writer);

    if (mpack_writer_destroy(&writer) != mpack_ok)
    {
        AEGLOG_ERROR("failed to serialize output options");
        return false;
    }

    bytes->assign(ptr, ptr + size);
    return true;
}
}
