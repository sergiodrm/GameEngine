#pragma once

#include "VoltEngine/Core/Window.h"

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
        void Init(const SWindowData& data);
        void Shutdown();

    private:
        SWindowData m_windowData;

        struct GLFWwindow* m_nativeWindow;

        /**
         * Statics
         */
        static bool s_GLFWInitialized;
    };
}
