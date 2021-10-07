#pragma once
#include <chrono>
#include <string>

#define PROFILE_SCOPE(scopeName) ::Volt::CScopeTimer scopeName##Timer(#scopeName);
#define PROFILE_FUNCTION() ::Volt::CScopeTimer functionTimer(__FUNCSIG__);

namespace Volt
{
    class CTime
    {
        using Clock = std::chrono::system_clock;
        using TimePoint = Clock::time_point;
        using Duration = std::chrono::duration<double, std::milli>;

    public:
        CTime();

        template <typename T = float>
        T GetElapsedTimeSeconds() const { return static_cast<T>(m_elapsedTime.count() * 0.001); }

        template <typename T = float>
        T GetElapsedTimeMilliseconds() const { return static_cast<T>(m_elapsedTime.count()); }

        float GetFrameRate() const { return static_cast<float>(1.0 / m_elapsedTime.count()); }

        void Update();

    private:
        TimePoint m_startTimePoint;
        TimePoint m_lastTimePoint;
        Duration m_elapsedTime;
    };

    class CScopeTimer
    {
        using Clock = std::chrono::high_resolution_clock;
        using TimePoint = Clock::time_point;
        using Duration = std::chrono::duration<double, std::milli>;

    public:
        CScopeTimer(const std::string& scopeName);
        ~CScopeTimer();

        CScopeTimer(const CScopeTimer&) = delete;
        CScopeTimer(const CScopeTimer&&) = delete;
        void operator=(const CScopeTimer&) = delete;

    protected:
        void Start();
        void Stop();
    private:
        TimePoint m_startTimePoint;
        Duration m_duration;
        std::string m_scopeName;
    };
}
