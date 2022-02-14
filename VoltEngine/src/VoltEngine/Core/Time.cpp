#include "Time.h"

#include "Log.h"


namespace Volt
{
    CTime::CTime()
        : m_elapsedTime(0.0)
    {
        m_startTimePoint = Clock::now();
        m_lastTimePoint = m_startTimePoint;
    }

    void CTime::Update()
    {
        const TimePoint now = Clock::now();
        m_elapsedTime = now - m_lastTimePoint;
        m_lastTimePoint = now;
    }

    void CElapseTimer::Start()
    {
        m_startTimePoint = Clock::now();
    }

    void CElapseTimer::Stop()
    {
        m_endTimePoint = Clock::now();
        m_duration = m_endTimePoint - m_startTimePoint;
    }

    CScopeTimer::CScopeTimer(const std::string& scopeName)
        : m_name(scopeName)
    {
        Start();
    }

    CScopeTimer::~CScopeTimer()
    {
        Stop();
        PrintResult();
    }

    void CScopeTimer::PrintResult() const
    {
        VOLT_LOG(Trace, "Timer: \"{0}\" elapsed time {1} ms.", m_name.c_str(), GetMilliseconds());
    }
}
