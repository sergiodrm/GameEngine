#include "UICommand.h"

#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>

#include "imgui.h"
#include "GLFW/glfw3.h"
#include "VoltEngine/Core/Application.h"
#include "VoltEngine/Core/Log.h"
#include "VoltEngine/Core/Window.h"

namespace Volt
{
    void CUICommand::Init()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();

        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.105f, 0.11f, 1.f);

        // Headers
        style.Colors[ImGuiCol_Header] = ImVec4(0.2f, 0.205f, 0.21f, 1.f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.3f, 0.305f, 0.31f, 1.f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.15f, 0.1505f, 0.151f, 1.f);

        // Buttons
        style.Colors[ImGuiCol_Button] = ImVec4(0.2f, 0.205f, 0.21f, 1.f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.305f, 0.31f, 1.f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.1505f, 0.151f, 1.f);

        // Frame background
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.205f, 0.21f, 1.f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3f, 0.305f, 0.31f, 1.f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.15f, 0.1505f, 0.151f, 1.f);

        // Tabs
        style.Colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.1505f, 0.151f, 1.f);
        style.Colors[ImGuiCol_TabHovered] = ImVec4(0.38f, 0.3805f, 0.381f, 1.f);
        style.Colors[ImGuiCol_TabActive] = ImVec4(0.28f, 0.2805f, 0.281f, 1.f);
        style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.1505f, 0.151f, 1.f);
        style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.2f, 0.205f, 0.21f, 1.f);

        // Title background
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.15f, 0.1505f, 0.151f, 1.f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.1505f, 0.151f, 1.f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.95f, 0.1505f, 0.951f, 1.f);

        CApplication& application = CApplication::Get();
        GLFWwindow* nativeWindow = static_cast<GLFWwindow*>(application.GetWindow()->GetNativeWindow());
        VOLT_ASSERT(nativeWindow, "ImGui hasn't access to native window.");

        ImGui_ImplGlfw_InitForOpenGL(nativeWindow, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void CUICommand::Shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void CUICommand::BeginFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void CUICommand::EndFrame()
    {
        ImGuiIO& io = ImGui::GetIO();
        CApplication& app = CApplication::Get();

        io.DisplaySize = ImVec2(static_cast<float>(app.GetWindow()->GetWidth()), static_cast<float>(app.GetWindow()->GetHeight()));

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backupContext = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backupContext);
        }
    }
}
