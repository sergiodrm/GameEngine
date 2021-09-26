#include "WindowsWindow.h"
#include "VoltEngine/VoltEngine.h"

#include <cassert>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "VoltEngine/Events/ApplicationEvent.h"

#define GET_WINDOW_USER_POINTER(var, window) \
    SWindowData* var = static_cast<SWindowData*>(glfwGetWindowUserPointer(window)); \
    if (!EXPAND(var)) { VOLT_LOG(Error, "Native window has not set user pointer data!"); return; }


namespace Volt
{
    bool CWindowsWindow::s_GLFWInitialized = false;

    static void GLFWErrorCallback(int32_t error, const char* description)
    {
        VOLT_LOG(Error, "GLFW error: ({0}) {1}", error, description);
    }

    CWindowsWindow::CWindowsWindow(const SWindowData& windowData)
        : m_windowData(windowData)
    {
        Init();
    }

    CWindowsWindow::~CWindowsWindow()
    {
        Shutdown();
    }

    void CWindowsWindow::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_nativeWindow);
    }

    void CWindowsWindow::Init()
    {
        VOLT_LOG(Info, "Creating window! {0}, {1}", m_windowData.Width, m_windowData.Height);

        if (!s_GLFWInitialized)
        {
            const int32_t success = glfwInit();
            assert(success);
            s_GLFWInitialized = true;

            glfwSetErrorCallback(&GLFWErrorCallback);
        }

        // Create window
        m_nativeWindow = glfwCreateWindow(m_windowData.Width, m_windowData.Height, m_windowData.Title.c_str(), nullptr, nullptr);
        assert(m_nativeWindow);

        glfwSetWindowUserPointer(m_nativeWindow, &m_windowData);

        glfwMakeContextCurrent(m_nativeWindow);
        VOLT_LOG(Trace, "OpenGL info:");
        //VOLT_LOG(Trace, "   - Vendor:   {0}", glGetString(GL_VENDOR));
        //VOLT_LOG(Trace, "   - Renderer: {0}", glGetString(GL_RENDERER));
        //VOLT_LOG(Trace, "   - Version:  {0}", glGetString(GL_VERSION));

        // Bind events
        glfwSetWindowCloseCallback(m_nativeWindow, [](GLFWwindow* window)
        {
            GET_WINDOW_USER_POINTER(windowData, window);
            CWindowCloseEvent e;
            windowData->EventFunction(e);
        });

        glfwSetFramebufferSizeCallback(m_nativeWindow, [](GLFWwindow* window, int32_t width, int32_t height)
        {
            GET_WINDOW_USER_POINTER(windowData, window);
            windowData->Width = static_cast<uint32_t>(width);
            windowData->Height = static_cast<uint32_t>(height);
            CWindowResizeEvent e(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
            windowData->EventFunction(e);
        });
    }

    void CWindowsWindow::Shutdown()
    {
        glfwDestroyWindow(m_nativeWindow);
    }
}

//#undef GET_WINDOW_USER_POINTER
