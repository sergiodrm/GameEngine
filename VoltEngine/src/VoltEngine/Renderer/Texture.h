#pragma once
#include "VoltEngine/Core/Log.h"
#include "VoltEngine/Resources/Resource.h"

namespace Volt
{
    enum class ETextureLoadType
    {
        File,
        Procedural
    };

    class ITexture : public IResource
    {
    public:
        static SharedPtr<ITexture> Create(IResourceManager* creator, const std::string& name, uint32_t id);

        ITexture(IResourceManager* creator, const std::string& name, uint32_t id)
            : IResource(creator, name, id), m_loadType(ETextureLoadType::File) {}

        virtual ~ITexture() = default;

        virtual uint32_t GetRendererID() const = 0;

        virtual void Bind(uint32_t slot = 0) const = 0;
        virtual void Unbind() const = 0;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual void SetWidth(uint32_t width) = 0;
        virtual void SetHeight(uint32_t height) = 0;

        virtual void SetData(void* data, uint32_t size) = 0;

        virtual bool operator==(const ITexture& other) const = 0;

        ETextureLoadType GetLoadType() const { return m_loadType; }
        void SetLoadType(ETextureLoadType type) { m_loadType = type; }

    protected:
        ETextureLoadType m_loadType;
    };
}
