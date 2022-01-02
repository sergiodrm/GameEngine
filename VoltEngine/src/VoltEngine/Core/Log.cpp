#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Volt
{
    SharedPtr<spdlog::logger> CLog::s_coreLogger = nullptr;
    SharedPtr<spdlog::logger> CLog::s_clientLogger = nullptr;

    void CLog::Init()
    {
        // Log pattern:
        // %^ -> start color section
        // %$ -> end color section
        spdlog::set_pattern("[%T][thread:%t]%^ %n: %v%$");
        s_coreLogger = spdlog::stdout_color_mt("Engine");
        s_coreLogger->set_level(spdlog::level::trace);
        s_clientLogger = spdlog::stdout_color_mt("Game");
        s_clientLogger->set_level(spdlog::level::trace);

        s_coreLogger->info("Logger initialized");
    }
}
