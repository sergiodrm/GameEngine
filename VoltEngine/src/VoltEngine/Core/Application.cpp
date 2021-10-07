#include "Application.h"

#include <cassert>

#include "Layer.h"
#include "Log.h"
#include "Time.h"
#include "Window.h"
#include "VoltEngine/Events/ApplicationEvent.h"
#include "VoltEngine/Renderer/RenderCommand.h"
#include "VoltEngine/Renderer/Renderer.h"

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

        CRenderer::Init();
    }

    CApplication::~CApplication()
    { }

    void CApplication::Run()
    {
        VOLT_LOG(Warning, "Run!");

        CTime mainLoopTimer;

        while (IsRunning())
        {
            mainLoopTimer.Update();
            if (!m_minimized)
            {
                for (CLayer* it : m_layerStack)
                {
                    it->OnUpdate(mainLoopTimer.GetElapsedTimeSeconds());
                }
            }
            m_window->OnUpdate();
        }
    }

    void CApplication::PushLayer(CLayer* layer)
    {
        m_layerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void CApplication::PopLayer(CLayer* layer)
    {
        m_layerStack.PopLayer(layer);
        layer->OnDetach();
    }

    void CApplication::PushOverlay(CLayer* overlay)
    {
        m_layerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    void CApplication::PopOverlay(CLayer* overlay)
    {
        m_layerStack.PopOverlay(overlay);
        overlay->OnDetach();
    }

    void CApplication::OnEvent(CEvent& e)
    {
        CEventDispatcher dispatcher(e);
        //VOLT_LOG(Info, "Event received! -> {0}", e.ToString());

        if (!dispatcher.Dispatch<CWindowClosedEvent>(BIND_FUNCTION(CApplication::OnWindowClosed)))
        {
            if (!dispatcher.Dispatch<CWindowResizedEvent>(BIND_FUNCTION(CApplication::OnWindowResized)))
            {
                // Send event to layers
                for (CLayer* it : m_layerStack)
                {
                    if (it->OnEvent(e))
                    {
                        break;
                    }
                }
            }
        }
    }

    bool CApplication::OnWindowClosed(CWindowClosedEvent& e)
    {
        m_running = false;
        return false;
    }

    bool CApplication::OnWindowResized(CWindowResizedEvent& e)
    {
        m_minimized = e.GetWidth() == 0 || e.GetHeight() == 0;
        CRenderer::OnWindowResize(e.GetWidth(), e.GetHeight());
        return false;
    }
}
