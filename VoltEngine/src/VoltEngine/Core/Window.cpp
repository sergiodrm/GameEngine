#include "Window.h"

#include "VoltEngine/Platform/Windows/WindowsWindow.h"

namespace Volt
{
    IWindow* IWindow::Create(const SWindowData& windowProps)
    {
#if VOLT_PLATFORM_WINDOWS
        return new CWindowsWindow(windowProps);
#else
#error This SO is not available to VoltEngine
#endif
    }
}
