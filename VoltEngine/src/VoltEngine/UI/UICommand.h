#pragma once

namespace Volt
{
    class CUICommand
    {
    public:
        static void Init();
        static void Shutdown();
        static void BeginFrame();
        static void EndFrame();
    private:
        static void InitStyle();
    };
}
