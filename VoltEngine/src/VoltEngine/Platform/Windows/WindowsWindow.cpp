#include "WindowsWindow.h"

#include <cassert>
#include <cstdio>

namespace Volt
{
    bool CWindowsWindow::s_GLFWInitialized = false;

    CWindowsWindow::CWindowsWindow(const SWindowData& windowData)
        : m_windowData(windowData)
    {
        Init(windowData);
    }

    CWindowsWindow::~CWindowsWindow()
    {
        Shutdown();
    }

    void CWindowsWindow::OnUpdate() {}

    void CWindowsWindow::Init(const SWindowData& data)
    { }

    void CWindowsWindow::Shutdown() {}
}
