#include "GameLayer.h"

#include "imgui.h"
#include "VoltEngine/Scene/Components/NativeScriptComponent.h"

#define TEXTURE_SAMPLE_FILEPATH "assets/textures/sample.png"


CGameLayer::CGameLayer(const std::string& name)
    : CLayer(name), m_viewportSize(0.f)
{}

void CGameLayer::OnAttach()
{
    CLayer::OnAttach();

#if 0
    m_tileSetTexture = Volt::ITexture::Create("assets/game/textures/RPGpack_sheet_2X.png");
    m_treeTexture = Volt::CSubTexture::Create(m_tileSetTexture, { 6.f, 0.f }, { 128.f, 128.f }, { 1.f, 1.f });

    m_camera = Volt::CreateRef<Volt::CCamera>();

#endif // 0

    m_framebuffer = Volt::IFramebuffer::Create({1280, 720});

    m_scene = Volt::CreateSharedPtr<Volt::CScene>();

    m_cameraEntity = m_scene->CreateEntity("CameraEntity");
    m_cameraEntity->AddComponent<Volt::CCameraComponent>(true);

    m_squareEntity = m_scene->CreateEntity("SquareEntity");
    m_squareEntity->AddComponent<Volt::CSpriteRenderComponent>(TEXTURE_SAMPLE_FILEPATH);
    m_squareEntity->AddComponent<Volt::CMovementScriptComponent>();

    Volt::CRenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.f});
}

void CGameLayer::OnUpdate(float elapsedSeconds)
{
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

void CGameLayer::OnUIRender()
{
    {
        const Volt::SRenderer2DStats& stats = Volt::CRenderer2D::GetStats();

        ImGui::Begin("VoltEngine");
        ImGui::Text("Welcome to Volt Engine!!!");
        ImGui::Separator();
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
