#pragma once
#include "VoltEngine/Renderer/Framebuffer.h"


namespace Volt
{
    class COpenGLFramebuffer : public IFramebuffer
    {
    public:
        COpenGLFramebuffer(const SFramebufferSpecification& spec);
        virtual ~COpenGLFramebuffer() override;

        virtual const SFramebufferSpecification& GetSpecification() const override { return m_specification; }
        virtual uint32_t GetColorAttachmentRendererID() const override { return m_colorAttachment; }
        virtual void Bind() const override;
        virtual void Unbind() const override;
        virtual void Resize(const glm::vec2& size) override;

        void Invalidate();

    private:
        uint32_t m_rendererID;
        uint32_t m_colorAttachment;
        uint32_t m_depthAttachment;
        SFramebufferSpecification m_specification;
    };
}
