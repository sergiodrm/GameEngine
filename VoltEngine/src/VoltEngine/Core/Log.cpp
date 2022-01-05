#include "Log.h"

#include <iostream>

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Volt
{
    void CCustomSpdlogSink::sink_it_(const spdlog::details::log_msg& msg)
    {
        spdlog::memory_buf_t formatted;
        formatter_->format(msg, formatted);
        std::cout << fmt::to_string(formatted);
        CLog::SubmitLog(msg, fmt::to_string(formatted));
    }

    void CCustomSpdlogSink::flush_()
    {
        std::cout << std::flush;
    }


    SharedPtr<spdlog::logger> CLog::s_coreLogger = nullptr;
    //SharedPtr<spdlog::logger> CLog::s_clientLogger = nullptr;
    std::vector<CLog::SLogRequest> CLog::s_logRegister;
    uint32_t CLog::s_logRegisterMaxSize = 50;

    CLog::EType CLog::LevelToType(spdlog::level::level_enum level)
    {
        switch (level)
        {
            case spdlog::level::trace: return Trace;
            case spdlog::level::debug: return Trace;
            case spdlog::level::info: return Info;
            case spdlog::level::warn: return Warning;
            case spdlog::level::err: return Error;
            default: return Error;
        }
    }

    void CLog::Init()
    {
        // Log pattern:
        // %^ -> start color section
        // %$ -> end color section
        spdlog::set_pattern("[%T][thread:%t]%^ %n: %v%$");
        //s_coreLogger = spdlog::stdout_color_mt("Engine");
        s_coreLogger = CCustomSpdlogSink::Create("Engine");
        s_coreLogger->set_level(spdlog::level::trace);
        //s_clientLogger = spdlog::stdout_color_mt("Game");
        //s_clientLogger = CCustomSpdlogSink::Create("Engine");
        //s_clientLogger->set_level(spdlog::level::trace);

        s_coreLogger->info("Logger initialized");
    }

    void CLog::SubmitLog(const spdlog::details::log_msg& msg, const std::string& formatted)
    {
        if (s_logRegister.size() == s_logRegisterMaxSize)
            s_logRegister.erase(s_logRegister.begin());

        const EType type = LevelToType(msg.level);
        s_logRegister.push_back({type, formatted});
    }
}
