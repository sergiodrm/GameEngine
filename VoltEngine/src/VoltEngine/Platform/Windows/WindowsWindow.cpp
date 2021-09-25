#include "WindowsWindow.h"

#include <cassert>
#include <cstdio>

#include "VoltEngine/Core/Log.h"

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
    {
        VOLT_LOG(Info, "Creating window! {0}, {1}", data.Width, data.Height);
    }

    void CWindowsWindow::Shutdown() {}
}
