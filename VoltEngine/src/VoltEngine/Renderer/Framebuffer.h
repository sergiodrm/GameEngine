#pragma once
#include "VoltEngine/Core/Core.h"
#include "glm/glm.hpp"

namespace Volt
{
    struct SFramebufferSpecification
    {
        uint32_t Width;
        uint32_t Height;
        uint32_t Samples = 1;

        bool SwapChainTarget = false;
    };

    class IFramebuffer
    {
    public:
        static Ref<IFramebuffer> Create(const SFramebufferSpecification& spec);

        virtual ~IFramebuffer() = default;

        virtual const SFramebufferSpecification& GetSpecification() const = 0;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual void Resize(const glm::vec2& size) = 0;
        virtual uint32_t GetColorAttachmentRendererID() const = 0;
    };
}
