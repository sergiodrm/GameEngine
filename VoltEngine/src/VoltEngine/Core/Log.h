#pragma once
#include "Core.h"
#include "spdlog/spdlog.h"

namespace Volt
{
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

        static void Init();

        static SharedPtr<spdlog::logger>& GetCoreLogger() { return s_coreLogger; }
        static SharedPtr<spdlog::logger>& GetClientLogger() { return s_clientLogger; }

        static SharedPtr<spdlog::logger>& CLog::GetLogger()
        {
#if VOLT_ENGINE
            return GetCoreLogger();
#else
            return GetClientLogger();
#endif
        }

        template <typename ... Args>
        static void Log(EType type, const char* format, Args&& ... args);

    private:
        static SharedPtr<spdlog::logger> s_coreLogger;
        static SharedPtr<spdlog::logger> s_clientLogger;
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
