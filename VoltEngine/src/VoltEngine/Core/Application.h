#pragma once
#include "Core.h"
#include "LayerStack.h"

namespace Volt
{
    class CTime;

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
        void Close();

        void PushLayer(CLayer* layer);
        void PopLayer(CLayer* layer);
        void PushOverlay(CLayer* overlay);
        void PopOverlay(CLayer* overlay);

        static CApplication& Get() { return *s_instance; }
        static const char* GetAppName() { return s_appName; }
        static uint32_t GetFrame() { return s_frameCounter; } // TODO this shouldn't be static

        SharedPtr<class IWindow>& GetWindow() { return m_window; }
        const SharedPtr<class IWindow>& GetWindow() const { return m_window; }

        const CTime& GetTimer() const { return *m_timer; }


    protected:
        void OnEvent(class CEvent& e);
        bool OnWindowClosed(class CWindowClosedEvent& e);
        bool OnWindowResized(class CWindowResizedEvent& e);

        bool IsRunning() const { return m_running; }

    private:
        static CApplication* s_instance;
        static const char* s_appName;
        static uint32_t s_frameCounter; // TODO this shouldn't be static

        SharedPtr<class IWindow> m_window;
        CLayerStack m_layerStack;
        bool m_running {true};
        bool m_minimized {false};

        CTime* m_timer;
    };
}
