#include "WindowsWindow.h"
#include "VoltEngine/VoltEngine.h"

#include <cassert>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "VoltEngine/Events/ApplicationEvent.h"
#include "VoltEngine/Events/KeyEvent.h"
#include "VoltEngine/Events/MouseEvent.h"
#include "VoltEngine/Renderer/GraphicsContext.h"

#define GET_WINDOW_USER_POINTER(var, window) \
    SWindowData* var = static_cast<SWindowData*>(glfwGetWindowUserPointer(window)); \
    if (!EXPAND(var)) { VOLT_LOG(Error, "Native window has not set user pointer data!"); return; }


namespace Volt
{
    bool CWindowsWindow::s_GLFWInitialized = false;
    uint32_t CWindowsWindow::s_activeWindows = 0;

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
        m_graphicsContext->SwapBuffers();
    }

    void CWindowsWindow::Init()
    {
        VOLT_LOG(Info, "Creating window! {0}, {1}", m_windowData.Width, m_windowData.Height);

        ++s_activeWindows;
        if (!s_GLFWInitialized)
        {
            const int32_t success = glfwInit();
            VOLT_ASSERT(success, "GLFW library couldn't be initialized!");
            s_GLFWInitialized = true;

            glfwSetErrorCallback(&GLFWErrorCallback);
        }

        // Create window
        m_nativeWindow = glfwCreateWindow(m_windowData.Width, m_windowData.Height, m_windowData.Title.c_str(), nullptr, nullptr);
        VOLT_ASSERT(m_nativeWindow, "GLFW library couldn't create native window!");

        glfwSetWindowUserPointer(m_nativeWindow, &m_windowData);

        m_graphicsContext = IGraphicsContext::Create();
        m_graphicsContext->Init(m_nativeWindow);


        // Bind events
        glfwSetWindowCloseCallback(m_nativeWindow, [](GLFWwindow* window)
        {
            GET_WINDOW_USER_POINTER(windowData, window);
            CWindowClosedEvent e;
            windowData->EventFunction(e);
        });

        glfwSetWindowSizeCallback(m_nativeWindow, [](GLFWwindow* window, int32_t width, int32_t height)
        {
            GET_WINDOW_USER_POINTER(windowData, window);
            windowData->Width = static_cast<uint32_t>(width);
            windowData->Height = static_cast<uint32_t>(height);
            CWindowResizedEvent e(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
            windowData->EventFunction(e);
        });

        glfwSetCursorPosCallback(m_nativeWindow, [](GLFWwindow* window, double x, double y)
        {
            GET_WINDOW_USER_POINTER(windowData, window);
            CMouseMovedEvent e(static_cast<float>(x), static_cast<float>(y));
            windowData->EventFunction(e);
        });

        glfwSetMouseButtonCallback(m_nativeWindow, [](GLFWwindow* window, int32_t button, int32_t action, int32_t)
        {
            GET_WINDOW_USER_POINTER(windowData, window);
            if (action == GLFW_PRESS || action == GLFW_REPEAT)
            {
                CMouseButtonPressedEvent e(button);
                windowData->EventFunction(e);
            }
            else if (action == GLFW_RELEASE)
            {
                CMouseButtonReleasedEvent e(button);
                windowData->EventFunction(e);
            }
        });

        glfwSetScrollCallback(m_nativeWindow, [](GLFWwindow* window, double x, double y)
        {
            GET_WINDOW_USER_POINTER(windowData, window);
            CMouseScrolledEvent e(static_cast<float>(x), static_cast<float>(y));
            windowData->EventFunction(e);
        });

        glfwSetKeyCallback(m_nativeWindow, [](GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
        {
            GET_WINDOW_USER_POINTER(windowData, window);
            if (action == GLFW_PRESS || action == GLFW_REPEAT)
            {
                CKeyPressedEvent e(key);
                windowData->EventFunction(e);
            }
            else if (action == GLFW_RELEASE)
            {
                CKeyReleasedEvent e(key);
                windowData->EventFunction(e);
            }
        });
    }

    void CWindowsWindow::Shutdown()
    {
        glfwDestroyWindow(m_nativeWindow);
        --s_activeWindows;
        if (s_activeWindows == 0)
        {
            glfwTerminate();
            s_GLFWInitialized = false;
        }
    }
}

//#undef GET_WINDOW_USER_POINTER
