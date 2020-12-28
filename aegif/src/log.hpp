#pragma once

// std
#include <string>
#include <chrono>

// AE
#include <SPBasic.h>

// spdlog
#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>

namespace aegif
{
void SetupLog(struct SPBasicSuite* pica_basicP);
void SetdownLog();

template <class... Args>
class __Block
{
public:
    __Block(spdlog::source_loc loc, spdlog::string_view_t fmt, Args... args) : srcLoc_(loc)
    {
        spdlog::memory_buf_t buf;
        fmt::format_to(buf, fmt, args...);
        msg_ = std::string(buf.data(), buf.size());
        spdlog::default_logger_raw()->log(srcLoc_, spdlog::level::trace, "<begin> {}", msg_);
        sw_ = spdlog::stopwatch();
    }
    ~__Block()
    {
        const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(sw_.elapsed()).count();
        spdlog::default_logger_raw()->log(srcLoc_, spdlog::level::trace, "<end> {} ({} [us])", msg_, elapsed);
    }

private:
    __Block(const __Block&) = delete;
    __Block& operator=(const __Block&) = delete;

    spdlog::stopwatch sw_;
    std::string msg_;
    spdlog::source_loc srcLoc_;
};

#define __AEGLOG_CAT(x, y) x##y
#define __AEGLOG_BLOCK_NAME2(x) __AEGLOG_CAT(block, x)
#define __AEGLOG_BLOCK_NAME __AEGLOG_BLOCK_NAME2(__LINE__)

#ifdef _AEGIF_ENABLE_LOG
#define AEGLOG_TRACE SPDLOG_TRACE
#define AEGLOG_DEBUG SPDLOG_DEBUG
#define AEGLOG_INFO SPDLOG_INFO
#define AEGLOG_WARN SPDLOG_WARN
#define AEGLOG_ERROR SPDLOG_ERROR
#define AEGLOG_CRITICAL SPDLOG_CRITICAL
#define AEGLOG_BLOCK(...)                                                                                    \
    aegif::__Block __AEGLOG_BLOCK_NAME(spdlog::source_loc{ __FILE__, __LINE__, SPDLOG_FUNCTION }, __VA_ARGS__)
#else
#undef SPDLOG_TRACE
#undef SPDLOG_DEBUG
#undef SPDLOG_INFO
#undef SPDLOG_WARN
#undef SPDLOG_ERROR
#undef SPDLOG_CRITICAL

#define AEGLOG_TRACE(...) (void) 0
#define AEGLOG_DEBUG(...) (void) 0
#define AEGLOG_INFO(...) (void) 0
#define AEGLOG_WARN(...) (void) 0
#define AEGLOG_ERROR(...) (void) 0
#define AEGLOG_CRITICAL(...) (void) 0
#define AEGLOG_BLOCK(...) (void) 0
#endif
}
