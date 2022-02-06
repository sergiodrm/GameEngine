#pragma once
#include "VoltEngine/Core/Core.h"
#include "glm/glm.hpp"

namespace Volt
{
    enum class EFramebufferTextureFormat : uint8_t
    {
        None = 0,
        RGBA8,
        RED_INTEGER,
        DEPTH24STENCIL8,
        Depth = DEPTH24STENCIL8
    };

    struct SFramebufferTextureSpecification
    {
        SFramebufferTextureSpecification() = default;

        SFramebufferTextureSpecification(EFramebufferTextureFormat textureFormat)
            : TextureFormat(textureFormat) {}

        EFramebufferTextureFormat TextureFormat {EFramebufferTextureFormat::None};
    };

    struct SFramebufferTextureAttachment
    {
        SFramebufferTextureAttachment() = default;

        SFramebufferTextureAttachment(std::initializer_list<SFramebufferTextureSpecification> list)
            : Attachment(list) {}

        std::vector<SFramebufferTextureSpecification> Attachment;
    };

    struct SFramebufferSpecification
    {
        uint32_t Width;
        uint32_t Height;
        uint32_t Samples = 1;

        SFramebufferTextureAttachment Attachments;

        bool SwapChainTarget = false;
    };

    class IFramebuffer
    {
    public:
        static SharedPtr<IFramebuffer> Create(const SFramebufferSpecification& spec);

        virtual ~IFramebuffer() = default;

        virtual const SFramebufferSpecification& GetSpecification() const = 0;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual void Resize(const glm::vec2& size) = 0;
        virtual void Resize(uint32_t width, uint32_t height) = 0;
        virtual int32_t ReadPixel(uint32_t attachmentIndex, int32_t screenX, int32_t screenY) = 0;

        virtual void ClearAttachment(uint32_t attachmentIndex, int32_t value) = 0;
        virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
    };
}
