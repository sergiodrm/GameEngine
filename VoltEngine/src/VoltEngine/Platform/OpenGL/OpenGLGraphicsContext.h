#pragma once

#include "VoltEngine/Core/Core.h"
#include "VoltEngine/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Volt
{
    class COpenGLGraphicsContext : public IGraphicsContext
    {
    public:
        COpenGLGraphicsContext();

        virtual void Init(void* windowContext) override;
        virtual void SwapBuffers() override;
        virtual void SetWindowContext(void* windowContext) override;
    private:
        GLFWwindow* m_window;
    };
}
