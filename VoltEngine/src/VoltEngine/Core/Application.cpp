#include "Application.h"

#include <cassert>
#include <cstdio>

#include "Window.h"

namespace Volt
{
    CApplication* CApplication::s_instance = nullptr;

    CApplication::CApplication()
    {
        printf("Creating application\n");

        assert(!s_instance);
        s_instance = this;

        // Create window
        m_window = IWindow::Create();
    }

    CApplication::~CApplication()
    {
        delete m_window;

        printf("Destroying application\n");
    }

    void CApplication::Run()
    {
        printf("Running application\n");
    }
}
