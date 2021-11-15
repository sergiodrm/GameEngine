#include "Renderer3D.h"

#include "Camera.h"
#include "Mesh.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"

namespace Volt
{
    constexpr const char* BasicShaderFilepath = "assets/shaders/Basic3D.glsl";

    struct SSceneBatchData
    {
        glm::mat4 ViewProjectionMatrix;

        SharedPtr<IShader> Shader;
    };

    static SSceneBatchData* BatchData {nullptr};

    void CRenderer3D::Init()
    {
        BatchData = new SSceneBatchData();
        BatchData->Shader = IShader::Create(BasicShaderFilepath);
    }

    void CRenderer3D::Shutdown()
    {
        BatchData->Shader.reset();
        delete BatchData;
    }

    void CRenderer3D::BeginScene(const CCamera& camera, const glm::mat4& transform)
    {
        BatchData->ViewProjectionMatrix = camera.GetProjection() * inverse(transform);
        BatchData->Shader->Bind();
    }

    void CRenderer3D::EndScene()
    {
        // TODO: batch rendering
    }

    void CRenderer3D::DrawMesh(const glm::mat4& transform, const SharedPtr<IMesh>& mesh)
    {
        const glm::mat4 mvp = BatchData->ViewProjectionMatrix * transform;
        BatchData->Shader->SetMat4("u_MVP", mvp);
        const SharedPtr<IVertexArray> vertexArray = mesh->GetVertexArray();
        CRenderCommand::DrawIndexed(vertexArray);
    }
}
