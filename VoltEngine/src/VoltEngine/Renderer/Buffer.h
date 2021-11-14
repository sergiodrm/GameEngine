#pragma once
#include "VoltEngine/Core/Log.h"

namespace Volt
{
    enum class EShaderDataType
    {
        None = 0,
        Float,
        Float2,
        Float3,
        Float4,
        Int,
        Int2,
        Int3,
        Int4,
        Mat3,
        Mat4,
        Bool
    };

    static uint32_t ShaderDataTypeSize(EShaderDataType type)
    {
        switch (type)
        {
            case EShaderDataType::Float: return 4;
            case EShaderDataType::Float2: return 4 * 2;
            case EShaderDataType::Float3: return 4 * 3;
            case EShaderDataType::Float4: return 4 * 4;
            case EShaderDataType::Mat3: return 4 * 3 * 3;
            case EShaderDataType::Mat4: return 4 * 4 * 4;
            case EShaderDataType::Int: return 4;
            case EShaderDataType::Int2: return 4 * 2;
            case EShaderDataType::Int3: return 4 * 3;
            case EShaderDataType::Int4: return 4 * 4;
            case EShaderDataType::Bool: return 1;
            default:
                VOLT_LOG(Error, "Unknown ShaderDataType");
                return 0;
        }
    }

    struct SBufferElement
    {
        SBufferElement(EShaderDataType type, const std::string& name, bool normalized = false)
            : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
        { }

        uint32_t GetComponentCount() const
        {
            switch (Type)
            {
                case EShaderDataType::Float: return 1;
                case EShaderDataType::Float2: return 2;
                case EShaderDataType::Float3: return 3;
                case EShaderDataType::Float4: return 4;
                case EShaderDataType::Mat3: return 3 * 3;
                case EShaderDataType::Mat4: return 4 * 4;
                case EShaderDataType::Int: return 1;
                case EShaderDataType::Int2: return 2;
                case EShaderDataType::Int3: return 3;
                case EShaderDataType::Int4: return 4;
                case EShaderDataType::Bool: return 1;
                default:
                    VOLT_LOG(Error, "Unknown ShaderDataType");
                    return 0;
            }
        }

        std::string Name;
        EShaderDataType Type;
        uint32_t Size;
        uint32_t Offset;
        bool Normalized;
    };

    class CBufferLayout
    {
    public:
        CBufferLayout() = default;

        CBufferLayout(const std::initializer_list<SBufferElement>& elements)
            : m_elements(elements), m_stride(0)
        {
            CalculateOffsetAndStride();
        }

        uint32_t GetStride() const { return m_stride; }
        const std::vector<SBufferElement>& GetElements() const { return m_elements; }

        using Iterator = std::vector<SBufferElement>::iterator;
        using ConstIterator = std::vector<SBufferElement>::const_iterator;
        Iterator begin() { return m_elements.begin(); }
        Iterator end() { return m_elements.end(); }
        ConstIterator begin() const { return m_elements.begin(); }
        ConstIterator end() const { return m_elements.end(); }

    private:
        void CalculateOffsetAndStride()
        {
            uint32_t offset = 0;
            m_stride = 0;
            for (SBufferElement& it : m_elements)
            {
                it.Offset = offset;
                offset += it.Size;
                m_stride += it.Size;
            }
        }

    private:
        std::vector<SBufferElement> m_elements;
        uint32_t m_stride;
    };

    class IVertexBuffer
    {
    public:
        static SharedPtr<IVertexBuffer> Create(float* vertices, uint32_t size);
        static SharedPtr<IVertexBuffer> Create(uint32_t size);

        virtual ~IVertexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetData(const void* data, uint32_t size) = 0;

        virtual void SetLayout(const CBufferLayout& layout) = 0;
        virtual const CBufferLayout& GetLayout() const = 0;
    };

    class IIndexBuffer
    {
    public:
        static SharedPtr<IIndexBuffer> Create(uint32_t* indices, uint32_t count);

        virtual ~IIndexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual uint32_t GetCount() const = 0;
    };
}
