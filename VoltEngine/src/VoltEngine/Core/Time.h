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

    class CElapseTimer
    {
    protected:
        using Clock = std::chrono::high_resolution_clock;
        using TimePoint = Clock::time_point;
        using Duration = std::chrono::duration<double, std::milli>;
    public:
        CElapseTimer() = default;
        CElapseTimer(const CElapseTimer&) = delete;
        CElapseTimer(const CElapseTimer&&) = delete;
        void operator=(const CElapseTimer&) = delete;

        void Start();
        void Stop();

        template <typename T = float>
        T GetMilliseconds() const { return static_cast<T>(m_duration.count()); }

        template <typename T = float>
        T GetSeconds() const { return static_cast<T>(m_duration.count()) * 0.001f; }

        uint64_t GetStartStamp() const { return m_startTimePoint.time_since_epoch().count(); }
        uint64_t GetEndStamp() const { return m_endTimePoint.time_since_epoch().count(); }

    private:
        TimePoint m_startTimePoint {};
        TimePoint m_endTimePoint {};
        Duration m_duration {};
    };


    class CScopeTimer : CElapseTimer
    {
    public:
        CScopeTimer(const std::string& scopeName);
        ~CScopeTimer();

        CScopeTimer(const CScopeTimer&) = delete;
        CScopeTimer(const CScopeTimer&&) = delete;
        void operator=(const CScopeTimer&) = delete;

    protected:
        void PrintResult() const;
    private:
        std::string m_name;
    };
}
