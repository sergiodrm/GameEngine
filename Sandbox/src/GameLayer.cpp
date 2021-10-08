#include "GameLayer.h"

#include "VoltEngine/Renderer/Buffer.h"
#include "VoltEngine/Renderer/Renderer.h"
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

    Volt::CRenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.f});
}

void CGameLayer::OnUpdate(float elapsedSeconds)
{
    PROFILE_SCOPE(GameLayer);
    Volt::CRenderCommand::Clear();

    glm::mat4 transform(1.f);
    {
        PROFILE_SCOPE(Logic);
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

        transform = translate(glm::mat4(1.f), m_position);
    }
    {
        PROFILE_SCOPE(Render);
        Volt::CRenderer::BeginScene(m_camera);
        m_shader->Bind();
        m_shader->SetMat4("u_ViewProjection", transform * m_camera->GetProjection());
        //m_shader->SetMat4("u_ModelTransform", transform);
        m_texture->Bind();
        m_vertexArray->Bind();
        Volt::CRenderCommand::DrawIndexed(m_vertexArray);
        Volt::CRenderer::EndScene();
    }
}

void CGameLayer::OnUIRender()
{
    {
        ImGui::Begin("VoltEngine");
        ImGui::Text("Welcome to Volt Engine!!!");
        ImGui::Separator();
        ImGui::End();
    }
}
