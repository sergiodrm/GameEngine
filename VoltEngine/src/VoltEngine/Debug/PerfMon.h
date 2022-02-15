#pragma once
#include <fstream>
#include <string>
#include "VoltEngine/Core/Application.h"
#include "VoltEngine/Core/Time.h"

#define PERFORMANCE_MONITORING 0
#if PERFORMANCE_MONITORING
#define INIT_PERF_MON()         ::Volt::CPerfMon::Init(::Volt::CApplication::GetAppName());
#define SHUTDOWN_PERF_MON()     ::Volt::CPerfMon::Shutdown(::Volt::CApplication::GetAppName());

#define BEGIN_PERF_MON(name)                                                                                            \
                                ::Volt::CPerfMon perfMon##name(::Volt::CApplication::GetAppName(), #name);              \
                                perfMon##name.Start();
#define END_PERF_MON(name)      perfMon##name.Stop();

#define SCOPE_PERF_MON(name)    ::Volt::CScopePerfMon scopePerfMon##name(::Volt::CApplication::GetAppName(), #name);
#else
#define INIT_PERF_MON()         
#define SHUTDOWN_PERF_MON()     
#define BEGIN_PERF_MON(name)    
#define END_PERF_MON(name)      
#define SCOPE_PERF_MON(name)    
#endif // PERFORMANCE_MONITORING


namespace Volt
{
    class CPerfMon
    {
    public:
        CPerfMon(const std::string& filepath, const std::string& name);
        virtual ~CPerfMon() = default;

        static void Init(const char* filepath);
        static void Shutdown(const char* filepath);

        void Start();
        void Stop();

    protected:
        void WriteHeader();
        void WriteBody();
        void WriteFooter();

    private:
        static uint32_t s_perfInstanceCounter;
        static std::ofstream s_stream;
        static bool s_firstEntry;

        CElapseTimer m_timer;

        std::string m_filepath;
        std::string m_name;
    };

    class CScopePerfMon : public CPerfMon
    {
    public:
        CScopePerfMon(const std::string& filepath, const std::string& name)
            : CPerfMon(filepath, name)
        {
            Start();
        }

        virtual ~CScopePerfMon() override
        {
            Stop();
        }
    };
}
