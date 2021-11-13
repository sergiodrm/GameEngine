#include "VoltEngine/Core/Input.h"
#include "GLFW/glfw3.h"
#include "VoltEngine/Core/Application.h"
#include "VoltEngine/Core/Log.h"
#include "VoltEngine/Core/Window.h"

namespace Volt
{
    bool IInput::IsKeyPressed(int32_t key)
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(CApplication::Get().GetWindow()->GetNativeWindow());
        const int32_t state = glfwGetKey(window, key);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool IInput::IsMouseButtonPressed(int32_t button)
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(CApplication::Get().GetWindow()->GetNativeWindow());
        const int32_t state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    float IInput::GetMouseX()
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(CApplication::Get().GetWindow()->GetNativeWindow());
        double x;
        glfwGetCursorPos(window, &x, nullptr);
        return static_cast<float>(x);
    }

    float IInput::GetMouseY()
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(CApplication::Get().GetWindow()->GetNativeWindow());
        double y;
        glfwGetCursorPos(window, nullptr, &y);
        return static_cast<float>(y);
    }

    void IInput::GetMousePosition(float& x, float& y)
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(CApplication::Get().GetWindow()->GetNativeWindow());
        double dx;
        double dy;
        glfwGetCursorPos(window, &dx, &dy);
        x = static_cast<float>(dx);
        y = static_cast<float>(dy);
    }
}
