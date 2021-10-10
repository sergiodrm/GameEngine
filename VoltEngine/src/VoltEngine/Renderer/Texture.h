#pragma once
#include "VoltEngine/Core/Log.h"

namespace Volt
{
    class ITexture
    {
    public:
        static Ref<ITexture> Create(uint32_t width, uint32_t height);
        static Ref<ITexture> Create(const std::string& filepath);

        virtual ~ITexture() = default;

        virtual uint32_t GetRendererID() const = 0;

        virtual void Bind(uint32_t slot = 0) const = 0;
        virtual void Unbind() const = 0;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;

        virtual void SetData(void* data, uint32_t size) = 0;

        virtual bool operator==(const ITexture& other) const = 0;
    };
}
