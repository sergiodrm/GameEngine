#pragma once

namespace Volt
{
    class CEvent;

    class CUICommand
    {
    public:
        static void Init();
        static void Shutdown();
        static void BeginFrame();
        static void EndFrame();
        static void OnEvent(CEvent& e);
        static void BlockEvents(bool block);
    private:
        static void InitStyle();

        static bool s_blockEvents;
    };
}
