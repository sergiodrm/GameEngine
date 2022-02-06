#include "EditorLayer.h"

#include "Gizmos.h"
#include "imgui.h"
#include "glm/detail/type_quat.hpp"
#include "glm/gtx/quaternion.hpp"
#include "Panels/LogPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/StatsPanel.h"
#include "VoltEngine/Renderer/VertexData.h"
#include "VoltEngine/Scene/Components/NativeScriptComponent.h"

#include "VoltEngine/Renderer/EditorCamera.h"
#include "VoltEngine/Scene/SceneSerializer.h"
#include "VoltEngine/UI/UICommand.h"

#define TEST_TEXTURE_PATH "assets/textures/sample.png"


void CEditorLayer::OnAttach()
{
    CLayer::OnAttach();

    Volt::SFramebufferSpecification specification {
            1280, 720, 1,
            {
                Volt::EFramebufferTextureFormat::RGBA8,
                Volt::EFramebufferTextureFormat::RED_INTEGER,
                Volt::EFramebufferTextureFormat::Depth
            }
        };
    m_framebuffer = Volt::IFramebuffer::Create(specification);

    m_scene = Volt::CreateSharedPtr<Volt::CScene>();

#if 0
    Volt::SharedPtr<Volt::IMesh> cubeMesh = CreateCube();

    for (int32_t x = -10; x < 10; ++x)
    {
        for (int32_t y = -10; y < 10; ++y)
        {
            char name[16];
            sprintf_s(name, "Cube_%d_%d", x + 10, y + 10);
            Volt::CEntity* cubeEntity = m_scene->CreateEntity(name);
            cubeEntity->AddComponent<Volt::CMeshComponent>(cubeMesh);
            cubeEntity->AddComponent<Volt::CRotateScriptComponent>();
            cubeEntity->GetComponent<Volt::CTransformComponent>()->SetPosition({ static_cast<float>(x) * 1.75f, static_cast<float>(y) * 1.75f, 0.f });
        }
    }
#endif // 0

    //Volt::SharedPtr<Volt::IMesh> cubeMesh = Volt::CMeshManager::Get().Load("assets/models/skull/12140_Skull_v3_L2.obj");

    //Volt::CEntity* cubeEntity = m_scene->CreateEntity("Skull");
    //cubeEntity->AddComponent<Volt::CMeshComponent>(cubeMesh);
    //cubeEntity->AddComponent<Volt::CRotateScriptComponent>();
    //cubeEntity->GetComponent<Volt::CTransformComponent>()->SetRotation({-90.f, 0.f, 0.f});

    m_sceneHierarchyPanel = Volt::CreateSharedPtr<Volt::CSceneHierarchyPanel>(m_scene);
    m_statsPanel = Volt::CreateSharedPtr<Volt::CStatsPanel>();
    m_logPanel = Volt::CreateSharedPtr<Volt::CLogPanel>();

    m_editorCamera = Volt::CreateSharedPtr<Volt::CEditorCamera>(45.f, 1.3337f, 0.1f, 10000.f);
    m_gizmo = Volt::CreateSharedPtr<Volt::CGizmo>(m_scene.get());

    Volt::CRenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.f});
}

void CEditorLayer::OnUpdate(float elapsedSeconds)
{
    CLayer::OnUpdate(elapsedSeconds);
    //PROFILE_SCOPE(GameLayer);


    const Volt::SFramebufferSpecification& spec = m_framebuffer->GetSpecification();
    if (m_viewportSize.x > 0.f && m_viewportSize.y > 0.f &&
        (spec.Width != static_cast<uint32_t>(m_viewportSize.x) || spec.Height != static_cast<uint32_t>(m_viewportSize.y)))
    {
        m_framebuffer->Resize(m_viewportSize);
        m_scene->OnViewportResize(spec.Width, spec.Height);
        m_editorCamera->SetViewportSize(m_viewportSize.x, m_viewportSize.y);
    }
    //if (m_viewportFocused)
    {
        //PROFILE_SCOPE(Render);
        m_editorCamera->OnUpdate(elapsedSeconds);
        m_framebuffer->Bind();
        m_framebuffer->ClearAttachment(1, -1);
        Volt::CRenderCommand::Clear();
        /*m_scene->OnUpdateRuntime(elapsedSeconds);*/
        m_scene->OnUpdateEditor(m_editorCamera);

        ImVec2 mouse = ImGui::GetMousePos();
        mouse.x -= m_viewportBounds[0].x;
        mouse.y -= m_viewportBounds[0].y;
        const glm::vec2 viewportSize = m_viewportBounds[1] - m_viewportBounds[0];
        mouse.y = viewportSize.y - mouse.y;
        const int32_t mouseX = static_cast<int32_t>(mouse.x);
        const int32_t mouseY = static_cast<int32_t>(mouse.y);
        VOLT_LOG(Info, "Mouse: {0}, {1}   {2}, {3}", mouseX, mouseY, viewportSize.x, viewportSize.y);
        if (mouse.x >= 0.f && mouse.y >= 0.f && mouse.x < viewportSize.x && mouse.y < viewportSize.y)
        {
            const int32_t pixelData = m_framebuffer->ReadPixel(1, mouseX, mouseY);
            VOLT_LOG(Info, "ReadPixel: {0}", pixelData);
        }

        m_framebuffer->Unbind();
    }
}

void CEditorLayer::OnUIRender()
{
    {
        static bool showDemoWindow = false;
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New scene", "Ctrl+N"))
                    NewScene();
                if (ImGui::MenuItem("Open file...", "Ctrl+O"))
                    LoadScene();
                if (ImGui::MenuItem("Save file as...", "Ctrl+S"))
                    SaveScene();
                if (ImGui::MenuItem("Exit"))
                    Volt::CApplication::Get().Close();


                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Options"))
            {
                if (showDemoWindow)
                {
                    if (ImGui::MenuItem("Hide imgui demo"))
                        showDemoWindow = false;
                }
                else
                {
                    if (ImGui::MenuItem("Show imgui demo"))
                        showDemoWindow = true;
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        if (showDemoWindow)
        {
            ImGui::ShowDemoWindow();
        }
    }

    m_sceneHierarchyPanel->OnUIRender();
    m_statsPanel->OnUIRender();
    m_logPanel->OnUIRender();
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.f, 0.f});
        ImGui::Begin("Viewport");
        m_viewportFocused = ImGui::IsWindowFocused();
        const bool viewportHovered = ImGui::IsWindowHovered();
        Volt::CUICommand::BlockEvents(!m_viewportFocused/* && !viewportHovered*/);

        const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        if (glm::vec2(viewportPanelSize.x, viewportPanelSize.y) != m_viewportSize && viewportPanelSize.x > 0 && viewportPanelSize.y > 0)
        {
            m_viewportSize = {viewportPanelSize.x, viewportPanelSize.y};
        }

        const ImVec2 viewportOffset = ImGui::GetCursorPos();
        const ImVec2 windowSize = ImGui::GetWindowSize();
        ImVec2 minBound = ImGui::GetWindowPos();
        minBound.x += viewportOffset.x;
        minBound.y += viewportOffset.y;
        const ImVec2 maxBound = {minBound.x + windowSize.x, minBound.y + windowSize.y};
        m_viewportBounds[0] = {minBound.x, minBound.y};
        m_viewportBounds[1] = {maxBound.x, maxBound.y};

        const uint32_t texID = m_framebuffer->GetColorAttachmentRendererID();
        ImGui::Image(reinterpret_cast<void*>(texID), viewportPanelSize, {0.f, 1.f}, {1.f, 0.f});

        //m_gizmo->DrawGridGizmo(glm::mat4(1.f), m_editorCamera->GetProjection(), m_editorCamera->GetViewMatrix());
        m_gizmo->DrawEntityTransformGizmo(m_sceneHierarchyPanel->GetSelection(),
                                          m_editorCamera->GetProjection(),
                                          m_editorCamera->GetViewMatrix());

        ImGui::End();
        ImGui::PopStyleVar();
    }
}

bool CEditorLayer::OnEvent(Volt::CEvent& e)
{
    m_editorCamera->OnEvent(e);
    m_gizmo->OnEvent(e);
    return false;
}

void CEditorLayer::NewScene()
{
    m_scene.reset();
    m_scene = Volt::CreateSharedPtr<Volt::CScene>();
}

void CEditorLayer::SaveScene()
{
    const std::string filepath = Volt::CFileDialogs::SaveFile("GameEngine Scene (*.scn)\0*.scn\0");
    Volt::CSceneSerializer sceneSerializer(m_scene.get());
    sceneSerializer.Serialize(filepath);
}

void CEditorLayer::LoadScene()
{
    const std::string filepath = Volt::CFileDialogs::LoadFile("GameEngine Scene (*.scn)\0*.scn\0");
    Volt::CSceneSerializer sceneSerializer(m_scene.get());
    sceneSerializer.Deserialize(filepath);
}
