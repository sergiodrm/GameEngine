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
#include "VoltEngine/UI/UICommand.h"

#define TEST_TEXTURE_PATH "assets/textures/sample.png"


void CEditorLayer::OnAttach()
{
    CLayer::OnAttach();

    m_framebuffer = Volt::IFramebuffer::Create({1280, 720});

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

        Volt::CRenderCommand::Clear();
        /*m_scene->OnUpdateRuntime(elapsedSeconds);*/
        m_scene->OnUpdateEditor(m_editorCamera);

        m_framebuffer->Unbind();
    }
}

void CEditorLayer::OnUIRender()
{
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
