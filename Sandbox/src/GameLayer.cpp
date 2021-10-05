#include "GameLayer.h"

#include "VoltEngine/Renderer/Buffer.h"
#include "VoltEngine/Renderer/Renderer.h"

CGameLayer::CGameLayer(const std::string& name)
    : CLayer(name)
{}

void CGameLayer::OnAttach()
{
    CLayer::OnAttach();

    float vertices[18] =
        {
            -1.f, 0.f, 0.f, 1.f, 0.f, 0.f,
            1.f, 0.f, 0.f, 1.f, 0.f, 1.f,
            0.f, 1.f, 0.f, 0.f, 1.f, 0.f,
        };

    Volt::Ref<Volt::IVertexBuffer> buffer = Volt::IVertexBuffer::Create(vertices, sizeof(vertices));
    buffer->SetLayout({
                          {Volt::EShaderDataType::Float3, "a_Position"},
                          {Volt::EShaderDataType::Float3, "a_Color"}
                      });

    uint32_t indices[3] =
        {
            0, 1, 2
        };
    const Volt::Ref<Volt::IIndexBuffer> indexBuffer = Volt::IIndexBuffer::Create(indices, 3);

    m_vertexArray = Volt::IVertexArray::Create();
    m_vertexArray->AddVertexBuffer(buffer);
    m_vertexArray->SetIndexBuffer(indexBuffer);

    m_shader = Volt::IShader::Create("assets/shaders/simple.glsl");
    m_shader->Bind();
    m_shader->SetFloat4("u_Color", {1.f, 0.f, 0.f, 1.f});

    m_camera = Volt::CreateRef<Volt::CCamera>();

    Volt::CRenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.f});
}

void CGameLayer::OnUpdate()
{
    Volt::CRenderCommand::Clear();

    Volt::CRenderer::BeginScene(m_camera);
    Volt::CRenderer::Submit(m_shader, m_vertexArray);
    Volt::CRenderer::EndScene();
}
