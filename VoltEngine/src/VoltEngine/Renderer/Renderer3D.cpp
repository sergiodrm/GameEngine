#include "Renderer3D.h"

#include "Batch.h"
#include "Camera.h"
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

        std::array<ITexture*, 32> TextureSlots;
        uint32_t TextureSlotIndex {1};
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

        BatchData->TextureSlots[0] = CTextureManager::Get().CreateResource<ITexture>("3DWhiteTexture");
        BatchData->TextureSlots[0]->SetLoadType(ETextureLoadType::Procedural);
        BatchData->TextureSlots[0]->SetWidth(1);
        BatchData->TextureSlots[0]->SetHeight(1);
        BatchData->TextureSlots[0]->Load();
        uint32_t whiteTextureData = 0xffffffff;
        BatchData->TextureSlots[0]->SetData(&whiteTextureData, sizeof(uint32_t));

        int32_t textureIndices[32];
        for (int32_t i = 0; i < 32; ++i)
        {
            textureIndices[i] = i;
        }
        BatchData->Shader->Bind();
        BatchData->Shader->SetIntArray("u_Textures", &textureIndices[0], 32);
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
        Flush();
    }

    void CRenderer3D::DrawMesh(const glm::mat4& transform, const SharedPtr<IMesh>& mesh)
    {
#if 0
        const glm::mat4 mvp = BatchData->ViewProjectionMatrix * transform;
        BatchData->Shader->SetMat4("u_MVP", mvp);
        const SharedPtr<IVertexArray> vertexArray = mesh->GetVertexArray();
        CRenderCommand::DrawIndexed(vertexArray);
#endif // 0

        if (!BatchData->Batch->CanAddTriangles(mesh->GetNumTriangles()))
        {
            Flush();
        }

        const SBatchConfig batchConfig {transform, 0};
        BatchData->Batch->AddTriangles(mesh->GetVertexData(), mesh->GetIndexData(), batchConfig);
        Stats.TriangleCount += mesh->GetNumTriangles();
        Stats.VertexCount += static_cast<uint32_t>(mesh->GetVertexData().size());
    }

    void CRenderer3D::UpdateBatch() {}

    void CRenderer3D::Flush()
    {
        if (!BatchData->Batch->IsEmpty())
        {
            BatchData->Batch->Render();
            BatchData->Batch->Clear();
            BatchData->TextureSlotIndex = 1;
            ++Stats.DrawCallCount;
        }
    }
}
