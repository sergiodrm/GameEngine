#pragma once
#include "Core.h"
#include "LayerStack.h"

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

        void PushLayer(CLayer* layer);
        void PopLayer(CLayer* layer);
        void PushOverlay(CLayer* overlay);
        void PopOverlay(CLayer* overlay);

        static CApplication& Get() { return *s_instance; }

        Ref<class IWindow>& GetWindow() { return m_window; }
        const Ref<class IWindow>& GetWindow() const { return m_window; }

    protected:
        void OnEvent(class CEvent& e);
        bool OnWindowClosed(class CWindowClosedEvent& e);
        bool OnWindowResized(class CWindowResizedEvent& e);

        bool IsRunning() const { return m_running; }

    private:
        static CApplication* s_instance;

        Ref<class IWindow> m_window;
        CLayerStack m_layerStack;
        bool m_running {true};
        bool m_minimized {false};
    };
}
