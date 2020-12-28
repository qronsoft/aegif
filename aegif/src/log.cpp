#include "log.hpp"

// std
#include <string>
#include <mutex>

// win32
#include <Windows.h>

// spdlog
#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/wincolor_sink.h>
#include <spdlog/sinks/basic_file_sink.h>

// AE
#include <AEGP_SuiteHandler.h>

namespace
{
template <typename Mutex>
class aegp_debug_log_sink : public spdlog::sinks::base_sink<Mutex>
{
public:
    aegp_debug_log_sink(struct SPBasicSuite* pica_basicP)
        : spdlog::sinks::base_sink<Mutex>(), suites_(pica_basicP)
    {
    }

protected:
    void sink_it_(const spdlog::details::log_msg& msg) override
    {
        const std::string loggerName = fmt::format("[{}]", msg.logger_name);
        const std::string level      = fmt::format("[{}]", spdlog::level::to_string_view(msg.level));
        spdlog::memory_buf_t formatted;
        base_sink<Mutex>::formatter_->format(msg, formatted);
        const std::string formattedMsg(fmt::to_string(formatted));
        suites_.UtilitySuite3()->AEGP_WriteToDebugLog(
            loggerName.c_str(), level.c_str(), formattedMsg.c_str());
    }

    void flush_() override {}

private:
    AEGP_SuiteHandler suites_;
};
using aegp_debug_log_sink_mt = aegp_debug_log_sink<std::mutex>;

void OpenOSConsole()
{
    // open command prompt
    AllocConsole();
    SetConsoleOutputCP(CP_UTF8);
    // remove close button from command prompt
    RemoveMenu(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_BYCOMMAND);
}
void CloseOSConsole()
{
    FreeConsole();
}

std::shared_ptr<spdlog::sinks::wincolor_stdout_sink_mt> CreateOSConsoleLogSink()
{
    auto consoleSink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
    consoleSink->set_level(spdlog::level::trace);
    consoleSink->set_pattern("[%t] [%n] [%^%l%$] [%H:%M:%S.%e] [%s:%#:%!] %v");
    return consoleSink;
}
std::shared_ptr<aegp_debug_log_sink_mt> CreateAEGPDebugLogSink(SPBasicSuite* pica_basicP)
{
    auto aegpDebugLogSink = std::make_shared<aegp_debug_log_sink_mt>(pica_basicP);
    aegpDebugLogSink->set_level(spdlog::level::trace);
    aegpDebugLogSink->set_pattern("[%H:%M:%S.%e] [%s:%#:%!] %v");
    return aegpDebugLogSink;
}
void SetupLogger(const std::string& loggerName, SPBasicSuite* pica_basicP)
{
    auto logger = std::make_shared<spdlog::logger>(loggerName);
#ifdef _AEGIF_USE_OS_CONSOLE_LOG
    logger->sinks().push_back(CreateOSConsoleLogSink());
#endif
    if (pica_basicP) { logger->sinks().push_back(CreateAEGPDebugLogSink(pica_basicP)); }
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::trace);
    spdlog::flush_every(std::chrono::seconds(3));
}
}

namespace aegif
{
void SetupLog(SPBasicSuite* pica_basicP)
{
#ifdef _AEGIF_ENABLE_LOG

#ifdef _AEGIF_USE_OS_CONSOLE_LOG
    OpenOSConsole();
#endif
    SetupLogger("aegif", pica_basicP);
    SPDLOG_INFO("start logging...");

#endif
}
void SetdownLog()
{
#ifdef _AEGIF_ENABLE_LOG

    SPDLOG_INFO("end logging...");
    spdlog::shutdown();
#ifdef _AEGIF_USE_OS_CONSOLE_LOG
    CloseOSConsole();
#endif

#endif
}
}
