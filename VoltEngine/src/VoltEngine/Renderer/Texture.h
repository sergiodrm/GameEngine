#pragma once
#include "VoltEngine/Core/Log.h"
#include "VoltEngine/AssetManager/Asset.h"

namespace Volt
{
    class ITexture : public IAsset
    {
    public:
        static SharedPtr<ITexture> Create();

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
    };
}
