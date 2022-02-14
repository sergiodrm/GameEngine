#include "PerfMon.h"

#include "VoltEngine/Core/Application.h"

namespace Volt
{
    std::ofstream CPerfMon::s_stream;
    bool CPerfMon::s_firstEntry = true;


    CPerfMon::CPerfMon(const std::string& filepath, const std::string& name)
        : m_filepath(filepath), m_name(name) {}

    void CPerfMon::Init(const char* filepath)
    {
        s_stream.open(filepath);
        s_stream << "{\"app\":\"game engine\",\"data\":[\n";
        s_stream.flush();
    }

    void CPerfMon::Shutdown(const char* filepath)
    {
        s_stream << "\n]}";
        s_stream.flush();
        s_stream.close();
    }

    void CPerfMon::Start()
    {
        WriteHeader();
        m_timer.Start();
    }

    void CPerfMon::Stop()
    {
        m_timer.Stop();
        WriteBody();
        WriteFooter();
    }

    void CPerfMon::WriteHeader()
    {
        if (!s_firstEntry)
            s_stream << ",";
        s_stream << "{";
        s_stream << "\"name\":\"" << m_name.c_str() << "\",";
        s_stream << "\"frame\":" << CApplication::Get().GetFrame() << ",";
        s_stream << "\"data\":[";
        s_stream.flush();
        s_firstEntry = true;
    }

    void CPerfMon::WriteBody()
    {
        const double elapsedTime = m_timer.GetMilliseconds<double>();
        s_stream << "],";
        s_stream << "\"start\":" << m_timer.GetStartStamp() << ",";
        s_stream << "\"end\":" << m_timer.GetEndStamp() << ",";
        s_stream << "\"elapsed\":" << elapsedTime;
        s_stream.flush();
    }

    void CPerfMon::WriteFooter()
    {
        s_stream << "}\n";
        s_stream.flush();
        s_firstEntry = false;
    }
}
