#include "OpenGLVertexArray.h"

#include "glad/glad.h"
#include "VoltEngine/Renderer/Buffer.h"

namespace Volt
{
    GLenum GetEnumOfShaderDataType(EShaderDataType dataType)
    {
        switch (dataType)
        {
            case EShaderDataType::Float: return GL_FLOAT;
            case EShaderDataType::Float2: return GL_FLOAT;
            case EShaderDataType::Float3: return GL_FLOAT;
            case EShaderDataType::Float4: return GL_FLOAT;
            case EShaderDataType::Mat3: return GL_FLOAT;
            case EShaderDataType::Mat4: return GL_FLOAT;
            case EShaderDataType::Int: return GL_INT;
            case EShaderDataType::Int2: return GL_INT;
            case EShaderDataType::Int3: return GL_INT;
            case EShaderDataType::Int4: return GL_INT;
            case EShaderDataType::Bool: return GL_BOOL;
            default:
                VOLT_LOG(Error, "Unknown ShaderDataType to OpenGLBaseType");
                return 0;
        }
    }

    COpenGLVertexArray::COpenGLVertexArray()
        : m_rendererID(0), m_indexBuffer(nullptr)
    {
        glCreateVertexArrays(1, &m_rendererID);
    }

    COpenGLVertexArray::~COpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &m_rendererID);
    }

    void COpenGLVertexArray::Bind() const
    {
        glBindVertexArray(m_rendererID);
    }

    void COpenGLVertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }

    void COpenGLVertexArray::AddVertexBuffer(const Ref<IVertexBuffer>& vertexBuffer)
    {
        VOLT_ASSERT(!vertexBuffer->GetLayout().GetElements().empty(), "Vertex buffer has no layout set.")
        glBindVertexArray(m_rendererID);
        vertexBuffer->Bind();
        uint32_t index = 0;
        for (const SBufferElement& element : vertexBuffer->GetLayout())
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index,
                                  element.GetComponentCount(),
                                  GetEnumOfShaderDataType(element.Type),
                                  element.Normalized ? GL_TRUE : GL_FALSE,
                                  vertexBuffer->GetLayout().GetStride(),
                                  reinterpret_cast<const void*>(element.Offset));
            ++index;
        }
        m_vertexBuffers.push_back(vertexBuffer);
    }

    void COpenGLVertexArray::SetIndexBuffer(const Ref<IIndexBuffer>& indexBuffer)
    {
        glBindVertexArray(m_rendererID);
        indexBuffer->Bind();
        m_indexBuffer = indexBuffer;
    }
}
