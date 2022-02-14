#include "Application.h"

#include <cassert>

#include "Layer.h"
#include "Log.h"
#include "Time.h"
#include "Window.h"
#include "VoltEngine/Events/ApplicationEvent.h"
#include "VoltEngine/Renderer/RenderCommand.h"
#include "VoltEngine/Renderer/Renderer.h"
#include "VoltEngine/AssetManager/AssetManager.h"
#include "VoltEngine/Debug/PerfMon.h"
#include "VoltEngine/UI/UICommand.h"

namespace Volt
{
    CApplication* CApplication::s_instance = nullptr;
    const char* CApplication::s_appName = "F:\\Programacion\\GameEngine\\json\\GameEnginePerformance.json"; // TODO receive name
    uint32_t CApplication::s_frameCounter = 0;

    CApplication::CApplication()
    {
        INIT_PERF_MON();
        SCOPE_PERF_MON(InitApp);
        // Initialize logger
        CLog::Init();
        VOLT_LOG(Trace, "Creating application...");

        assert(!s_instance);
        s_instance = this;

        // Create window
        SWindowData windowSpec {"Game Engine", 1920, 1080};
        windowSpec.EventFunction = BIND_FUNCTION(CApplication::OnEvent);
        m_window = IWindow::Create(windowSpec);

        // Create singletons
        CAssetManager::Create();

        // Initialize systems
        CRenderer::Init();
        CUICommand::Init();

        m_timer = new CTime();
    }

    CApplication::~CApplication()
    {
        {
            SCOPE_PERF_MON(DestroyApp)
            m_layerStack.Clear();
            delete m_timer;
            CUICommand::Shutdown();
            CRenderer::Shutdown();
            CAssetManager::Destroy();
            s_instance = nullptr;
        }
        SHUTDOWN_PERF_MON();
    }

    void CApplication::Run()
    {
        SCOPE_PERF_MON(RunApp);
        VOLT_LOG(Warning, "Run!");

        s_frameCounter = 0;
        while (IsRunning())
        {
            m_timer->Update();
            CAssetManager::Get().ProcessRequests();
            if (!m_minimized)
            {
                // Layers update
                BEGIN_PERF_MON(LayersUpdate);
                for (CLayer* it : m_layerStack)
                {
                    it->OnUpdate(m_timer->GetElapsedTimeSeconds());
                }
                END_PERF_MON(LayersUpdate);

                // Layers UI
                BEGIN_PERF_MON(UIUpdate);
                CUICommand::BeginFrame();
                for (CLayer* it : m_layerStack)
                {
                    it->OnUIRender();
                }
                CUICommand::EndFrame();
                END_PERF_MON(UIUpdate);
            }
            BEGIN_PERF_MON(WindowUpdate);
            m_window->OnUpdate();
            END_PERF_MON(WindowUpdate);
            ++s_frameCounter;
        }
    }

    void CApplication::Close()
    {
        m_running = false;
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
        //VOLT_LOG(Trace, "Event: {0}", e.ToString().c_str());
        CEventDispatcher dispatcher(e);
        dispatcher.Dispatch<CWindowClosedEvent>(BIND_FUNCTION(CApplication::OnWindowClosed));
        dispatcher.Dispatch<CWindowResizedEvent>(BIND_FUNCTION(CApplication::OnWindowResized));
        CUICommand::OnEvent(e);
        if (!e.Handled)
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
