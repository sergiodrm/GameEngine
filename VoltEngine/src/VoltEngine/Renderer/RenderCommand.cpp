#include "RenderCommand.h"

#include "VoltEngine/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Volt
{
    IRendererAPI* CRenderCommand::s_rendererAPI = nullptr;

    void CRenderCommand::Init()
    {
        assert(!s_rendererAPI);

        switch (IRendererAPI::GetAPI())
        {
            case IRendererAPI::API::OpenGL:
                {
                    s_rendererAPI = new COpenGLRendererAPI();
                }
                break;
            default:
                VOLT_LOG(Error, "Unknown Renderer API");
                assert(false);
        }

        s_rendererAPI->Init();
    }

    void CRenderCommand::Shutdown()
    {
        delete s_rendererAPI;
    }

    void CRenderCommand::Clear()
    {
        s_rendererAPI->Clear();
    }

    void CRenderCommand::SetClearColor(const glm::vec4& color)
    {
        s_rendererAPI->SetClearColor(color);
    }

    void CRenderCommand::DrawIndexed(const Ref<IVertexArray>& vertexArray, uint32_t indexCount)
    {
        s_rendererAPI->DrawIndexed(vertexArray, indexCount);
    }
}
