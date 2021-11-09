#include "GameLayer.h"

#include "imgui.h"
#include "VoltEngine/Scene/Components/NativeScriptComponent.h"


CGameLayer::CGameLayer(const std::string& name)
    : CLayer(name), m_viewportSize(0.f), m_position(0.f)
{}

void CGameLayer::OnAttach()
{
    CLayer::OnAttach();

    m_texture = Volt::ITexture::Create("assets/textures/sample.png");
    m_tileSetTexture = Volt::ITexture::Create("assets/game/textures/RPGpack_sheet_2X.png");
    m_treeTexture = Volt::CSubTexture::Create(m_tileSetTexture, {6.f, 0.f}, {128.f, 128.f}, {1.f, 1.f});

    m_camera = Volt::CreateRef<Volt::CCamera>();

    m_framebuffer = Volt::IFramebuffer::Create({1280, 720});

    m_scene = Volt::CreateRef<Volt::CScene>();

    m_cameraEntity = m_scene->CreateEntity("CameraEntity");
    Volt::CCameraComponent* cameraComponent = m_cameraEntity->AddComponent<Volt::CCameraComponent>();
    cameraComponent->SetPrimary(true);

    m_squareEntity = m_scene->CreateEntity("SquareEntity");
    m_squareEntity->AddComponent<Volt::CSpriteRenderComponent>(glm::vec4(1.f, 0.f, 0.f, 1.f));
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
        m_camera->SetViewportSize(spec.Width, spec.Height);
        m_scene->OnViewportResize(spec.Width, spec.Height);
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

        //Volt::CRenderer2D::BeginScene(*m_camera, transform);
        //Volt::CRenderer2D::DrawTexture({0.f, 0.f}, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}, m_treeTexture);
        //Volt::CRenderer2D::EndScene();

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
