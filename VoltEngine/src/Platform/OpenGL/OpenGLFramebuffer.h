#pragma once
#include "VoltEngine/Renderer/Framebuffer.h"


namespace Volt
{
    class COpenGLFramebuffer : public IFramebuffer
    {
    public:
        COpenGLFramebuffer(const SFramebufferSpecification& spec);
        virtual ~COpenGLFramebuffer() override;


        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void Resize(const glm::vec2& size) override;
        virtual void Resize(uint32_t width, uint32_t height) override;
        virtual int32_t ReadPixel(uint32_t attachmentIndex, int32_t screenX, int32_t screenY) override;

        virtual void ClearAttachment(uint32_t attachmentIndex, int32_t value) override;
        virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override;
        virtual const SFramebufferSpecification& GetSpecification() const override { return m_specification; }

    protected:
        void Invalidate();
        void DeleteFramebuffers();

    private:
        uint32_t m_rendererID;
        SFramebufferSpecification m_specification;

        std::vector<SFramebufferTextureSpecification> m_colorAttachmentSpecification;
        SFramebufferTextureSpecification m_depthAttachmentSpecification;

        std::vector<uint32_t> m_colorAttachments;
        uint32_t m_depthAttachment {0};
    };
}
