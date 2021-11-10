#include "EditorLayer.h"

#include "imgui.h"
#include "Panels/SceneHierarchyPanel.h"
#include "VoltEngine/Scene/Components/NativeScriptComponent.h"

void CEditorLayer::OnAttach()
{
    CLayer::OnAttach();

    m_framebuffer = Volt::IFramebuffer::Create({1280, 720});

    m_scene = Volt::CreateRef<Volt::CScene>();

    m_cameraEntity = m_scene->CreateEntity("CameraEntity");
    m_cameraEntity->AddComponent<Volt::CCameraComponent>(true);

    m_squareEntity = m_scene->CreateEntity("SquareEntity");
    m_squareEntity->AddComponent<Volt::CSpriteRenderComponent>(nullptr, glm::vec4(0.4f, 0.2f, 0.3f, 1.f));
    m_squareEntity->AddComponent<Volt::CMovementScriptComponent>();

    m_sceneHierarchyPanel = Volt::CreateRef<Volt::CSceneHierarchyPanel>(m_scene);

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
    }
    {
        //PROFILE_SCOPE(Render);

        m_framebuffer->Bind();
        Volt::CRenderCommand::Clear();
        m_scene->OnUpdate(elapsedSeconds);
        m_framebuffer->Unbind();
    }
}

void CEditorLayer::OnUIRender()
{
    m_sceneHierarchyPanel->OnUIRender();
    {
        const Volt::SRenderer2DStats& stats = Volt::CRenderer2D::GetStats();

        ImGui::Begin("Stats");
        ImGui::Text("- Draw calls: %d", stats.DrawCalls);
        ImGui::Text("- Quad count: %d", stats.QuadCount);
        ImGui::Text("- Vertices:   %d", stats.GetVerticesCount());
        ImGui::Text("- Indices:    %d", stats.GetIndicesCount());
        ImGui::Text("- Triangles:  %d", stats.GetTrianglesCount());

        ImGui::End();
    }
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.f, 0.f});
        ImGui::Begin("Viewport");

        const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        if (glm::vec2(viewportPanelSize.x, viewportPanelSize.y) != m_viewportSize && viewportPanelSize.x > 0 && viewportPanelSize.y > 0)
        {
            m_viewportSize = {viewportPanelSize.x, viewportPanelSize.y};
        }

        const uint32_t texID = m_framebuffer->GetColorAttachmentRendererID();
        ImGui::Image(reinterpret_cast<void*>(texID), viewportPanelSize, {0.f, 1.f}, {1.f, 0.f});
        ImGui::End();
        ImGui::PopStyleVar();
    }
}
