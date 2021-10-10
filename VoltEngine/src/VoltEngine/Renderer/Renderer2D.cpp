#include "Renderer2D.h"

#include "Buffer.h"
#include "Camera.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VoltEngine/Core/Core.h"

namespace Volt
{
    struct SQuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoords;
        float TexIndex;
        float TilingFactor;
    };

    struct SRenderer2DData
    {
        static constexpr uint32_t MaxQuads = 1000;
        static constexpr uint32_t MaxVertices = MaxQuads * 4;
        static constexpr uint32_t MaxIndices = MaxQuads * 6;
        static constexpr uint32_t MaxTextureSlots = 32;
        static constexpr uint32_t QuadVertexCount = 4;
        static constexpr glm::vec4 QuadVertexPositions[] = {
                {-0.5f, -0.5f, 0.f, 1.f},
                {0.5f, -0.5f, 0.f, 1.f},
                {0.5f, 0.5f, 0.f, 1.f},
                {-0.5f, 0.5f, 0.f, 1.f}
            };

        Ref<IVertexArray> QuadVertexArray;
        Ref<IVertexBuffer> QuadVertexBuffer;
        Ref<IShader> QuadShader;
        Ref<ITexture> WhiteTexture;

        uint32_t QuadIndexCount = 0;
        SQuadVertex* QuadVertexBase = nullptr;
        SQuadVertex* QuadVertexPtr = nullptr;

        uint32_t TextureSlotIndex = 1;
        std::array<Ref<ITexture>, MaxTextureSlots> TextureSlots;
    };

    static SRenderer2DData RendererData;

    void CRenderer2D::Init()
    {
        RendererData.QuadVertexBuffer = IVertexBuffer::Create(SRenderer2DData::MaxVertices * sizeof(SQuadVertex));
        RendererData.QuadVertexBuffer->SetLayout({
                                                     {EShaderDataType::Float3, "a_Position"},
                                                     {EShaderDataType::Float4, "a_Color"},
                                                     {EShaderDataType::Float2, "a_TexCoords"},
                                                     {EShaderDataType::Float, "a_TexIndex"},
                                                     {EShaderDataType::Float, "a_TilingFactor"}
                                                 });
        RendererData.QuadVertexArray = IVertexArray::Create();
        RendererData.QuadVertexArray->AddVertexBuffer(RendererData.QuadVertexBuffer);

        uint32_t* quadIndices = new uint32_t[SRenderer2DData::MaxIndices];
        uint32_t offset = 0;
        for (uint32_t index = 0; index < SRenderer2DData::MaxIndices; index += 6)
        {
            quadIndices[index + 0] = offset + 0;
            quadIndices[index + 1] = offset + 1;
            quadIndices[index + 2] = offset + 2;

            quadIndices[index + 3] = offset + 0;
            quadIndices[index + 4] = offset + 2;
            quadIndices[index + 5] = offset + 3;

            offset += 4;
        }
        Ref<IIndexBuffer> indexBuffer = IIndexBuffer::Create(quadIndices, SRenderer2DData::MaxIndices);

        RendererData.QuadVertexArray->SetIndexBuffer(indexBuffer);
        delete[] quadIndices;

        RendererData.QuadVertexBase = new SQuadVertex[SRenderer2DData::MaxVertices];
        RendererData.QuadVertexPtr = RendererData.QuadVertexBase;

        // Textures
        RendererData.WhiteTexture = ITexture::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        RendererData.WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));

        int32_t samplers[SRenderer2DData::MaxTextureSlots];
        for (int32_t i = 0; i < SRenderer2DData::MaxTextureSlots; ++i)
        {
            samplers[i] = i;
        }

        RendererData.QuadShader = IShader::Create("assets/shaders/QuadTexture.glsl");
        RendererData.QuadShader->Bind();
        RendererData.QuadShader->SetIntArray("u_Texture", samplers, SRenderer2DData::MaxIndices);

        RendererData.TextureSlots[0] = RendererData.WhiteTexture;
    }

    void CRenderer2D::Shutdown()
    {
        delete[] RendererData.QuadVertexBase;
    }

    void CRenderer2D::BeginScene(const CCamera& camera, const glm::mat4& transform)
    {
        const glm::mat4 viewProjection = camera.GetProjection() * transform;

        RendererData.QuadShader->Bind();
        RendererData.QuadShader->SetMat4("u_ViewProjection", viewProjection);

        Reset();
    }

    void CRenderer2D::EndScene()
    {
        const uint32_t dataSize = static_cast<uint32_t>(
            reinterpret_cast<uint8_t*>(RendererData.QuadVertexPtr)
            - reinterpret_cast<uint8_t*>(RendererData.QuadVertexBase));
        RendererData.QuadVertexBuffer->SetData(RendererData.QuadVertexBase, dataSize);
        Flush();
    }

    void CRenderer2D::DrawQuad(const glm::vec2& position, const glm::vec4& color)
    {
        DrawQuad({position.x, position.y, 0.f}, color);
    }

    void CRenderer2D::DrawQuad(const glm::vec3& position, const glm::vec4& color)
    {
        const glm::mat4 transform = translate(glm::mat4(1.f), position)
            * rotate(glm::mat4(1.f), 0.f, {1.f, 0.f, 0.f})
            * rotate(glm::mat4(1.f), 0.f, {0.f, 1.f, 0.f})
            * rotate(glm::mat4(1.f), 0.f, {0.f, 0.f, 1.f})
            * scale(glm::mat4(1.f), {1.f, 1.f, 1.f});
        DrawQuad(transform, color);
    }

    void CRenderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
    {
        UpdateBatch();

        const float textureIndex = 0.f;
        const float tilingFactor = 1.f;
        const glm::vec2 texCoords[] = {{0.f, 0.f}, {1.f, 0.f}, {1.f, 1.f}, {0.f, 1.f}};
        for (uint32_t index = 0; index < SRenderer2DData::QuadVertexCount; ++index)
        {
            RendererData.QuadVertexPtr->Position = transform * SRenderer2DData::QuadVertexPositions[index];
            RendererData.QuadVertexPtr->Color = color;
            RendererData.QuadVertexPtr->TexCoords = texCoords[index];
            RendererData.QuadVertexPtr->TexIndex = textureIndex;
            RendererData.QuadVertexPtr->TilingFactor = tilingFactor;
            ++RendererData.QuadVertexPtr;
        }
        RendererData.QuadIndexCount += 6;
    }


    void CRenderer2D::Flush()
    {
        if (RendererData.QuadIndexCount != 0)
        {
            for (uint32_t index = 0; index < RendererData.TextureSlotIndex; ++index)
            {
                RendererData.TextureSlots[index]->Bind(index);
            }

            CRenderCommand::DrawIndexed(RendererData.QuadVertexArray, RendererData.QuadIndexCount);
        }
    }

    void CRenderer2D::Reset()
    {
        RendererData.QuadIndexCount = 0;
        RendererData.QuadVertexPtr = RendererData.QuadVertexBase;
        RendererData.TextureSlotIndex = 1;
    }

    void CRenderer2D::UpdateBatch()
    {
        if (RendererData.QuadIndexCount >= SRenderer2DData::MaxIndices)
        {
            EndScene();
            Reset();
        }
    }
}
