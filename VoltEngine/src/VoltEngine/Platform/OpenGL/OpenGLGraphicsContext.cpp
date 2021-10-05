#include "OpenGLGraphicsContext.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "VoltEngine/Core/Log.h"
#include "VoltEngine/Core/Window.h"

namespace Volt
{
    COpenGLGraphicsContext::COpenGLGraphicsContext()
        : m_window(nullptr) {}

    void COpenGLGraphicsContext::Init(void* windowContext)
    {
        m_window = static_cast<GLFWwindow*>(windowContext);

        glfwMakeContextCurrent(m_window);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        VOLT_ASSERT(status, "Failed to initialize GLAD");

        VOLT_LOG(Info, "OpenGL Info: ");
        VOLT_LOG(Info, "  - Vendor:   {0}", glGetString(GL_VENDOR));
        VOLT_LOG(Info, "  - Renderer: {0}", glGetString(GL_RENDERER));
        VOLT_LOG(Info, "  - Version:  {0}", glGetString(GL_VERSION));
    }

    void COpenGLGraphicsContext::SwapBuffers()
    {
        glfwSwapBuffers(m_window);
    }

    void COpenGLGraphicsContext::SetWindowContext(void* windowContext)
    {
        m_window = static_cast<GLFWwindow*>(windowContext);
    }
}
