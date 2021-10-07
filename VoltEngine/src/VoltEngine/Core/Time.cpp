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

    CScopeTimer::CScopeTimer(const std::string& scopeName)
        : m_duration(0.0), m_scopeName(scopeName)
    {
        Start();
    }

    CScopeTimer::~CScopeTimer()
    {
        Stop();
        VOLT_LOG(Trace, "Timer: \"{0}\" elapsed time {1} ms.", m_scopeName.c_str(), m_duration.count());
    }

    void CScopeTimer::Start()
    {
        m_startTimePoint = Clock::now();
    }

    void CScopeTimer::Stop()
    {
        const TimePoint end = Clock::now();
        m_duration = end - m_startTimePoint;
    }
}
