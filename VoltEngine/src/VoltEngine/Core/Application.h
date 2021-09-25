#pragma once

namespace Volt
{
    class CApplication
    {
    public:
        CApplication();
        CApplication(const CApplication&) = delete;
        CApplication(const CApplication&&) = delete;
        void operator=(const CApplication&) = delete;
        void operator=(const CApplication&&) = delete;
        virtual ~CApplication();

        void Run();

    private:
        static CApplication* s_instance;

        class IWindow* m_window;
    };
}
