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
        m_window = IWindow::Create();
    }

    CApplication::~CApplication()
    {
        delete m_window;

        VOLT_LOG(Trace, "Creating application...");
    }

    void CApplication::Run()
    {
        VOLT_LOG(Warning, "Run!");
    }
}
