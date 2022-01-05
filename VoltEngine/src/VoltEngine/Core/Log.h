#pragma once
#include <vector>

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/base_sink.h"

namespace Volt
{
    class CCustomSpdlogSink : public spdlog::sinks::base_sink<std::mutex>
    {
    public:
        static std::shared_ptr<spdlog::logger> Create(const std::string& loggerName)
        {
            return spdlog::create<CCustomSpdlogSink>(loggerName);
        }

    protected:
        virtual void sink_it_(const spdlog::details::log_msg& msg) override;
        virtual void flush_() override;
    };

    class CLog
    {
    public:
        enum EType
        {
            Warning,
            Error,
            Fatal,
            Trace,
            Info
        };

        static EType LevelToType(spdlog::level::level_enum level);

        struct SLogRequest
        {
            EType LogType;
            std::string Log;
        };

        static void Init();

        static SharedPtr<spdlog::logger>& GetCoreLogger() { return s_coreLogger; }
        static SharedPtr<spdlog::logger>& GetLogger() { return GetCoreLogger(); }

        template <typename ... Args>
        static void Log(EType type, const char* format, Args&& ... args);

        static void SubmitLog(const spdlog::details::log_msg& msg, const std::string& formatted);

        static const std::vector<SLogRequest>& GetLogRegister() { return s_logRegister; }
        static uint32_t GetLogRegisterMaxSize() { return s_logRegisterMaxSize; }

    private:
        static SharedPtr<spdlog::logger> s_coreLogger;
        //static SharedPtr<spdlog::logger> s_clientLogger;
        static std::vector<SLogRequest> s_logRegister;
        static uint32_t s_logRegisterMaxSize;
    };

    template <typename ... Args>
    void CLog::Log(EType type, const char* format, Args&&... args)
    {
        switch (type)
        {
            case Warning: GetLogger()->warn(format, std::forward<Args>(args)...);
                break;
            case Error: GetLogger()->error(format, std::forward<Args>(args)...);
                break;
            case Fatal: GetLogger()->error(format, std::forward<Args>(args)...);
                break;
            case Trace: GetLogger()->trace(format, std::forward<Args>(args)...);
                break;
            case Info: GetLogger()->info(format, std::forward<Args>(args)...);
                break;
            default: GetLogger()->error("Unknown log category");
        }
    }
}

// Core log macros

#define VOLT_LOG(type, ...) ::Volt::CLog::Log(Volt::CLog::##type, __VA_ARGS__);
#define VOLT_ASSERT(assertion, msg, ...) if (!EXPAND(assertion)) {VOLT_LOG(Fatal, msg, __VA_ARGS__); assert(false);}
