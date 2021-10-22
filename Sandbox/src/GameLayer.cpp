#include "GameLayer.h"

#include "imgui.h"


CGameLayer::CGameLayer(const std::string& name)
    : CLayer(name), m_position(0.f)
{}

void CGameLayer::OnAttach()
{
    CLayer::OnAttach();

    m_texture = Volt::ITexture::Create("assets/textures/sample.png");

    m_camera = Volt::CreateRef<Volt::CCamera>();

    m_framebuffer = Volt::IFramebuffer::Create({1280, 720});

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
        m_camera->SetViewport(spec.Width, spec.Height);
    }

    glm::mat4 transform(1.f);
    {
        //PROFILE_SCOPE(Logic);
        if (Volt::IInput::IsKeyPressed(Volt::KeyCodes::O))
        {
            m_camera->SetProjectionType(Volt::CCamera::EProjectionType::Orthographic);
        }
        else if (Volt::IInput::IsKeyPressed(Volt::KeyCodes::P))
        {
            m_camera->SetProjectionType(Volt::CCamera::EProjectionType::Perspective);
        }

        if (Volt::IInput::IsKeyPressed(Volt::KeyCodes::S))
        {
            m_position.y += 5.f * elapsedSeconds;
        }
        if (Volt::IInput::IsKeyPressed(Volt::KeyCodes::W))
        {
            m_position.y -= 5.f * elapsedSeconds;
        }
        if (Volt::IInput::IsKeyPressed(Volt::KeyCodes::A))
        {
            m_position.x += 5.f * elapsedSeconds;
        }
        if (Volt::IInput::IsKeyPressed(Volt::KeyCodes::D))
        {
            m_position.x -= 5.f * elapsedSeconds;
        }
    }
    {
        //PROFILE_SCOPE(Render);
        transform = translate(glm::mat4(1.f), m_position);

        m_framebuffer->Bind();

        Volt::CRenderCommand::Clear();

        Volt::CRenderer2D::BeginScene(*m_camera, transform);

        const uint32_t width = 20;
        const uint32_t height = 20;
        static float rotation = 0.f;
        rotation += 45.f * elapsedSeconds;
        for (float x = 0; x < static_cast<float>(width); ++x)
        {
            for (float y = 0; y < static_cast<float>(height); ++y)
            {
                const glm::vec3 pos = {x * 1.2f, y * 1.2f, 0.f};
                Volt::CRenderer2D::DrawQuad(pos,
                                            {0.f, 0.f, rotation},
                                            {1.f, 1.f},
                                            {pos.x / static_cast<float>(width), pos.y / static_cast<float>(height), 0.f, 1.f});
            }
        }
        Volt::CRenderer2D::EndScene();

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
        ImGui::DragFloat3("Camera position", &m_position[0]);
        ImGui::Checkbox("Draw", &m_draw);
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
