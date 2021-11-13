#include "Window.h"

#include "Platform/Windows/WindowsWindow.h"

namespace Volt
{
    Ref<IWindow> IWindow::Create(const SWindowData& windowProps)
    {
#if VOLT_PLATFORM_WINDOWS
        return CreateRef<CWindowsWindow>(windowProps);
#else
#error This SO is not available to VoltEngine
#endif
    }
}
