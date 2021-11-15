#include "Renderer3D.h"

#include "Batch.h"
#include "Camera.h"
#include "Mesh.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexData.h"

namespace Volt
{
    constexpr const char* BasicShaderFilepath = "assets/shaders/Basic3D.glsl";

    struct SSceneBatchData
    {
        glm::mat4 ViewProjectionMatrix;

        SharedPtr<IShader> Shader;
        SharedPtr<IBatch> Batch;
    };

    static SSceneBatchData* BatchData {nullptr};

    void CRenderer3D::Init()
    {
        BatchData = new SSceneBatchData();
        BatchData->Shader = IShader::Create(BasicShaderFilepath);
        BatchData->Batch = IBatch::Create(1000);
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
        BatchData->Shader->SetMat4("u_ViewProjection", BatchData->ViewProjectionMatrix);
    }

    void CRenderer3D::EndScene()
    {
        if (!BatchData->Batch->IsEmpty())
        {
            BatchData->Batch->Render();
            BatchData->Batch->Clear();
        }
    }

    void CRenderer3D::DrawMesh(const glm::mat4& transform, const SharedPtr<IMesh>& mesh)
    {
#if 0
        const glm::mat4 mvp = BatchData->ViewProjectionMatrix * transform;
        BatchData->Shader->SetMat4("u_MVP", mvp);
        const SharedPtr<IVertexArray> vertexArray = mesh->GetVertexArray();
        CRenderCommand::DrawIndexed(vertexArray);
#endif // 0


        if (BatchData->Batch->CanAddTriangles(mesh->GetNumTriangles()))
        {
            std::vector<SVertexData> vertexData = mesh->GetVertexData();
            for (SVertexData& it : vertexData)
            {
                it.Position = transform * glm::vec4(it.Position.x, it.Position.y, it.Position.z, 1.f);
            }
            BatchData->Batch->AddTriangles(vertexData, mesh->GetIndexData());
        }
        else
        {
            VOLT_LOG(Error, "Can add more triangles to batch renderer!");
        }
    }
}
