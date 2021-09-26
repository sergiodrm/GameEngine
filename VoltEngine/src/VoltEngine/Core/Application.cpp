#include "Application.h"

#include <cassert>

#include "Log.h"
#include "Window.h"
#include "VoltEngine/Events/ApplicationEvent.h"

namespace Volt
{
    CApplication* CApplication::s_instance = nullptr;

    CApplication::CApplication()
    {
        // Initialize logger
        CLog::Init();
        VOLT_LOG(Trace, "Creating application...");

        assert(!s_instance);
        s_instance = this;

        // Create window
        SWindowData windowSpec;
        windowSpec.EventFunction = BIND_FUNCTION(CApplication::OnEvent);
        m_window = IWindow::Create(windowSpec);
    }

    CApplication::~CApplication()
    {
        delete m_window;
    }

    void CApplication::Run()
    {
        VOLT_LOG(Warning, "Run!");

        while (IsRunning())
        {
            m_window->OnUpdate();
        }
    }

    void CApplication::OnEvent(CEvent& e)
    {
        CEventDispatcher dispatcher(e);
        VOLT_LOG(Info, "Event received! -> {0}", e.ToString());
        dispatcher.Dispatch<CWindowCloseEvent>(BIND_FUNCTION(CApplication::OnWindowClosed));
        dispatcher.Dispatch<CWindowResizeEvent>(BIND_FUNCTION(CApplication::OnWindowResized));
    }

    bool CApplication::OnWindowClosed(CWindowCloseEvent& e)
    {
        m_running = false;
        return false;
    }

    bool CApplication::OnWindowResized(CWindowResizeEvent& e)
    {
        return false;
    }
}
