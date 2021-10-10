#include "GameLayer.h"

#include "imgui.h"


CGameLayer::CGameLayer(const std::string& name)
    : CLayer(name), m_position(0.f)
{}

void CGameLayer::OnAttach()
{
    CLayer::OnAttach();

    float vertices[] =
        {
            -0.5f, -0.5f, 0.5f, 1.f, 0.f, 0.f, 0.f, 0.f,
            0.5f, -0.5f, 0.5f, 1.f, 0.f, 0.f, 1.f, 0.f,
            0.5f, 0.5f, 0.5f, 1.f, 0.f, 0.f, 1.f, 1.f,
            -0.5f, 0.5f, 0.5f, 1.f, 0.f, 0.f, 0.f, 1.f
        };

    Volt::Ref<Volt::IVertexBuffer> buffer = Volt::IVertexBuffer::Create(vertices, sizeof(vertices));
    buffer->SetLayout({
                          {Volt::EShaderDataType::Float3, "a_Position"},
                          {Volt::EShaderDataType::Float3, "a_Color"},
                          {Volt::EShaderDataType::Float2, "a_TexCoord"}
                      });

    uint32_t indices[] =
        {
            0, 1, 2,
            0, 2, 3
        };
    const Volt::Ref<Volt::IIndexBuffer> indexBuffer = Volt::IIndexBuffer::Create(indices, 6);

    m_vertexArray = Volt::IVertexArray::Create();
    m_vertexArray->AddVertexBuffer(buffer);
    m_vertexArray->SetIndexBuffer(indexBuffer);

    m_texture = Volt::ITexture::Create("assets/textures/sample.png");

    m_shader = Volt::IShader::Create("assets/shaders/simple.glsl");
    m_shader->Bind();
    m_shader->SetMat4("u_ModelTransform", glm::mat4(1.f));
    m_texture->Bind();

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

        if (Volt::IInput::IsKeyPressed(Volt::KeyCodes::W))
        {
            m_position.y += 5.f * elapsedSeconds;
        }
        if (Volt::IInput::IsKeyPressed(Volt::KeyCodes::S))
        {
            m_position.y -= 5.f * elapsedSeconds;
        }
        if (Volt::IInput::IsKeyPressed(Volt::KeyCodes::D))
        {
            m_position.x += 5.f * elapsedSeconds;
        }
        if (Volt::IInput::IsKeyPressed(Volt::KeyCodes::A))
        {
            m_position.x -= 5.f * elapsedSeconds;
        }
    }
    {
        //PROFILE_SCOPE(Render);

        m_framebuffer->Bind();

        Volt::CRenderCommand::Clear();

        Volt::CRenderer2D::BeginScene(*m_camera, glm::mat4(1.f));
        Volt::CRenderer2D::DrawQuad(m_position, {1.f, 0.f, 0.f, 1.f});
        Volt::CRenderer2D::EndScene();

        m_framebuffer->Unbind();
    }
}

void CGameLayer::OnUIRender()
{
    {
        ImGui::Begin("VoltEngine");
        ImGui::Text("Welcome to Volt Engine!!!");
        ImGui::Separator();
        ImGui::DragFloat3("Square position", &m_position[0]);
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
