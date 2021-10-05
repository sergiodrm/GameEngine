#pragma once

#include "VoltEngine/Core/Window.h"

struct GLFWwindow;

namespace Volt
{
    class CWindowsWindow : public IWindow
    {
    public:
        CWindowsWindow(const SWindowData& windowData);
        virtual ~CWindowsWindow() override;

        /** Begin IWindow interface */
        virtual void OnUpdate() override;
        virtual uint32_t GetWidth() const override { return m_windowData.Width; }
        virtual uint32_t GetHeight() const override { return m_windowData.Height; }
        virtual void* GetNativeWindow() const override { return m_nativeWindow; }
        virtual void SetEventFunction(const SWindowData::WindowEvent& eventFunction) override { m_windowData.EventFunction = eventFunction; }
        /** End IWindow interface */

    protected:
        void Init();
        void Shutdown();

    private:
        SWindowData m_windowData;

        GLFWwindow* m_nativeWindow;
        Ref<class IGraphicsContext> m_graphicsContext;

        /**
         * Statics
         */
        static bool s_GLFWInitialized;
        static uint32_t s_activeWindows;
    };
}
