#include "Application.h"

#include <cassert>
#include <cstdio>

#include "Log.h"
#include "Window.h"

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

        VOLT_LOG(Trace, "Creating application...");
    }

    void CApplication::Run()
    {
        VOLT_LOG(Warning, "Run!");

        while (true)
        {
            m_window->OnUpdate();
        }
    }

    void CApplication::OnEvent(CEvent& e) {}
}
