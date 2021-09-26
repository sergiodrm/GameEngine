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

    protected:
        void OnEvent(class CEvent& e);
        bool OnWindowClosed(class CWindowClosedEvent& e);
        bool OnWindowResized(class CWindowResizedEvent& e);

        bool IsRunning() const { return m_running; }

    private:
        static CApplication* s_instance;

        class IWindow* m_window;

        bool m_running {true};
    };
}
