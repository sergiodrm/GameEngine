#include "Renderer3D.h"

#include "Batch.h"
#include "Camera.h"
#include "Material.h"
#include "Mesh.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "TextureManager.h"
#include "VertexArray.h"
#include "VertexData.h"

namespace Volt
{
    SRenderStats CRenderer3D::Stats;

    constexpr const char* BasicShaderFilepath = "assets/shaders/Basic3D.glsl";

    struct SSceneBatchData
    {
        glm::mat4 ViewProjectionMatrix {1.f};

        SharedPtr<IShader> Shader;
        SharedPtr<IBatch> Batch;
    };

    static SSceneBatchData* BatchData {nullptr};

    uint32_t SRenderStats::GetIndexCount() const
    {
        return TriangleCount * 3;
    }

    uint32_t SRenderStats::GetVertexUsedMemory() const
    {
        return sizeof(SVertexData) * VertexCount;
    }

    uint32_t SRenderStats::GetIndexUsedMemory() const
    {
        return sizeof(uint32_t) * GetIndexCount();
    }

    void SRenderStats::Reset()
    {
        VertexCount = 0;
        TriangleCount = 0;
        DrawCallCount = 0;
    }


    void CRenderer3D::Init()
    {
        constexpr uint32_t BatchSize = 2000;
        BatchData = new SSceneBatchData();
        BatchData->Shader = IShader::Create(BasicShaderFilepath);
        BatchData->Batch = IBatch::Create(BatchSize);
        Stats.BatchSize = BatchSize;
    }

    void CRenderer3D::Shutdown()
    {
        BatchData->Shader.reset();
        delete BatchData;
    }

    void CRenderer3D::BeginScene(const CCamera& camera, const glm::mat4& transform)
    {
        Stats.Reset();

        BatchData->ViewProjectionMatrix = camera.GetProjection() * inverse(transform);
        BatchData->Shader->Bind();
        BatchData->Shader->SetMat4("u_ViewProjection", BatchData->ViewProjectionMatrix);
    }

    void CRenderer3D::EndScene()
    {
#if 0
        Flush();
#endif // 0
    }

    void CRenderer3D::DrawMesh(const glm::mat4& transform, const SharedPtr<IMesh>& mesh)
    {
#if 1
        const glm::mat4 mvp = BatchData->ViewProjectionMatrix * transform;
        BatchData->Shader->SetMat4("u_MVP", mvp);
        mesh->GetMaterial()->Prepare(BatchData->Shader);
        const SharedPtr<IVertexArray>& vertexArray = mesh->GetVertexArray();
        CRenderCommand::DrawIndexed(vertexArray);
#else
        if (!BatchData->Batch->CanAddTriangles(mesh->GetNumTriangles()))
        {
            Flush();
        }

        const SBatchConfig batchConfig {transform, 0};
        BatchData->Batch->AddTriangles(mesh->GetVertexData(), mesh->GetIndexData(), batchConfig);
        Stats.TriangleCount += mesh->GetNumTriangles();
        Stats.VertexCount += static_cast<uint32_t>(mesh->GetVertexData().size());
#endif // 0
    }

    void CRenderer3D::UpdateBatch() {}

    void CRenderer3D::Flush()
    {
        if (!BatchData->Batch->IsEmpty())
        {
            BatchData->Batch->Render();
            BatchData->Batch->Clear();
            ++Stats.DrawCallCount;
        }
    }
}
