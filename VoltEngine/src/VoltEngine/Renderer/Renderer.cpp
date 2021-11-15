#include "Renderer.h"

#include "Camera.h"
#include "RenderCommand.h"
#include "Renderer2D.h"
#include "Renderer3D.h"

namespace Volt
{
    CRenderer::SSceneData CRenderer::s_sceneData;

    void CRenderer::Init()
    {
        CRenderCommand::Init();
        CRenderer2D::Init();
        CRenderer3D::Init();
    }

    void CRenderer::Shutdown()
    {
        CRenderer3D::Shutdown();
        CRenderer2D::Shutdown();
        CRenderCommand::Shutdown();
    }

    void CRenderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        CRenderCommand::SetViewport(0, 0, width, height);
    }

    void CRenderer::BeginScene(const SharedPtr<CCamera>& camera)
    {
        s_sceneData.SceneProjection = camera->GetProjection();
    }

    void CRenderer::EndScene() {}

    void CRenderer::Submit(const SharedPtr<IShader>& shader, const SharedPtr<IVertexArray>& vertexArray, uint32_t indexCount)
    {
        shader->Bind();
        shader->SetMat4("u_ViewProjection", s_sceneData.SceneProjection);
        vertexArray->Bind();
        CRenderCommand::DrawIndexed(vertexArray, indexCount);
    }
}
