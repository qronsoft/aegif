#include "output_options.hpp"

// std
#include <sstream>

// msgpack
#include <msgpack.hpp>
#include <msgpack/type.hpp>

// aegif
#include "log.hpp"

namespace aegif
{
OutputOptions::OutputOptions() = default;

bool OutputOptions::Load(const char* bytes, size_t size) try
{
    Data newData;

    auto oh         = msgpack::unpack(bytes, size);
    const auto& obj = oh.get();

    if (obj.type != msgpack::type::MAP)
    {
        AEGLOG_ERROR("invalid serialized data");
        return false;
    }

    for (uint32_t i = 0, size = obj.via.map.size; i < size; ++i)
    {
        const auto kv = obj.via.map.ptr + i;
        if (kv->key.type != msgpack::type::STR)
        {
            AEGLOG_WARN("non string key found");
            continue;
        }

        const auto key = kv->key.as<std::string>();
        if (key == "version")
        {
            newData.version = kv->val.as<decltype(Data::version)>();
        }
        else if (key == "loop")
        {
            newData.loop = kv->val.as<decltype(Data::loop)>();
        }
        else if (key == "fast")
        {
            newData.fast = kv->val.as<decltype(Data::fast)>();
        }
        else if (key == "quality")
        {
            newData.quality = kv->val.as<decltype(Data::quality)>();
        }
        else if (key == "encoderP")
        {
            newData.encoderP = kv->val.as<decltype(Data::encoderP)>();
        }
        else
        {
            AEGLOG_WARN("unexpected key found: {}", key);
        }
    }
    data_ = newData;
    return true;
}
catch (...)
{
    AEGLOG_ERROR("failed to load output options");
    return false;
}

bool OutputOptions::Load(const std::vector<char>& bytes)
{
    return Load(bytes.data(), bytes.size());
}

bool OutputOptions::Serialize(std::vector<char>* bytes) const try
{
    if (bytes == nullptr) return false;

    std::stringstream ss;
    msgpack::pack(&ss, data_);
    const auto str = ss.str();
    bytes->assign(str.begin(), str.end());
    return true;
}
catch (...)
{
    AEGLOG_CRITICAL("failed to serialize output options");
    return false;
}
}
