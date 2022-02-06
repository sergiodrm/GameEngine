#include "OpenGLFramebuffer.h"

#include "glad/glad.h"
#include "VoltEngine/Core/Log.h"

namespace Volt
{
    static constexpr uint32_t MaxFramebufferSize = 8192;

    namespace Utils
    {
        static GLenum TextureTarget(bool multisampled)
        {
            return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        }

        static void CreateTexture(bool multisampled, uint32_t* outID, uint32_t count)
        {
            glCreateTextures(TextureTarget(multisampled), count, outID);
        }

        static void BindTexture(bool multisampled, uint32_t id)
        {
            glBindTexture(TextureTarget(multisampled), id);
        }

        static void AttachColorTexture(uint32_t id, int32_t samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int32_t index)
        {
            const bool multisampled = samples > 1;
            if (multisampled)
            {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
        }

        static void AttachDepthTexture(uint32_t id, int32_t samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
        {
            const bool multisampled = samples > 1;
            if (multisampled)
            {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
            }
            else
            {
                glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
        }

        static bool IsDepthFormat(EFramebufferTextureFormat format)
        {
            switch (format)
            {
                case EFramebufferTextureFormat::DEPTH24STENCIL8: return true;
                default: return false;
            }
        }

        static GLenum TextureFormatToGL(EFramebufferTextureFormat format)
        {
            switch (format)
            {
                case EFramebufferTextureFormat::RGBA8: return GL_RGBA8;
                case EFramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
                default: return 0;
            }
        }
    }

    COpenGLFramebuffer::COpenGLFramebuffer(const SFramebufferSpecification& spec)
        : m_rendererID(0), m_specification(spec)
    {
        for (const SFramebufferTextureSpecification& format : m_specification.Attachments.Attachment)
        {
            if (!Utils::IsDepthFormat(format.TextureFormat))
                m_colorAttachmentSpecification.emplace_back(format);
            else
                m_depthAttachmentSpecification = format;
        }
        Invalidate();
    }

    COpenGLFramebuffer::~COpenGLFramebuffer()
    {
        DeleteFramebuffers();
    }

    void COpenGLFramebuffer::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
        glViewport(0, 0, m_specification.Width, m_specification.Height);
    }

    void COpenGLFramebuffer::Unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void COpenGLFramebuffer::Resize(const glm::vec2& size)
    {
        Resize(static_cast<uint32_t>(size.x), static_cast<uint32_t>(size.y));
    }

    void COpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
    {
        if (width == 0 || height == 0 || width > MaxFramebufferSize || height > MaxFramebufferSize)
        {
            VOLT_LOG(Warning, "Framebuffer couldn't be resized to [{0}, {1}]", width, height);
        }
        m_specification.Width = width;
        m_specification.Height = height;
        Invalidate();
    }

    int32_t COpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int32_t screenX, int32_t screenY)
    {
        VOLT_ASSERT(attachmentIndex < m_colorAttachments.size(), "Invalid attachment index");
        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
        int32_t pixelData = 0;
        glReadPixels(screenX, screenY, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
        return pixelData;
    }

    void COpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int32_t value)
    {
        VOLT_ASSERT(attachmentIndex < m_colorAttachments.size(), "Invalid attachment index");
        const SFramebufferTextureSpecification& spec = m_colorAttachmentSpecification[attachmentIndex];
        glClearTexImage(m_colorAttachments[attachmentIndex], 0, Utils::TextureFormatToGL(spec.TextureFormat), GL_INT, &value);
    }

    uint32_t COpenGLFramebuffer::GetColorAttachmentRendererID(uint32_t index) const
    {
        VOLT_ASSERT(index < m_colorAttachments.size(), "Index out of memory");
        return m_colorAttachments[index];
    }

    void COpenGLFramebuffer::Invalidate()
    {
        if (m_rendererID)
            DeleteFramebuffers();

        glCreateFramebuffers(1, &m_rendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);

        const bool multisampled = m_specification.Samples > 1;
        if (!m_colorAttachmentSpecification.empty())
        {
            m_colorAttachments.resize(m_colorAttachmentSpecification.size());
            Utils::CreateTexture(multisampled, m_colorAttachments.data(), m_colorAttachments.size());
            for (size_t index = 0; index < m_colorAttachmentSpecification.size(); ++index)
            {
                Utils::BindTexture(multisampled, m_colorAttachments[index]);

                const GLenum internalFormat = m_colorAttachmentSpecification[index].TextureFormat == EFramebufferTextureFormat::RGBA8 ? GL_RGBA8 : GL_R32I;
                const GLenum format = m_colorAttachmentSpecification[index].TextureFormat == EFramebufferTextureFormat::RGBA8 ? GL_RGBA : GL_RED_INTEGER;

                Utils::AttachColorTexture(m_colorAttachments[index],
                                          m_specification.Samples,
                                          internalFormat, format,
                                          m_specification.Width, m_specification.Height,
                                          static_cast<int32_t>(index));
            }
        }
        if (m_depthAttachmentSpecification.TextureFormat != EFramebufferTextureFormat::None)
        {
            Utils::CreateTexture(multisampled, &m_depthAttachment, 1);
            Utils::BindTexture(multisampled, m_depthAttachment);
            Utils::AttachDepthTexture(m_depthAttachment,
                                      m_specification.Samples,
                                      GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT,
                                      m_specification.Width, m_specification.Height);
        }
        if (m_colorAttachments.size() > 1)
        {
            VOLT_ASSERT(m_colorAttachments.size() <= 4, "Cannot be more than 4 color attachments");
            GLenum buffers[4] = {
                    GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                    GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3
                };
            glDrawBuffers(static_cast<GLsizei>(m_colorAttachments.size()), buffers);
        }
        else if (m_colorAttachments.empty())
        {
            glDrawBuffer(GL_NONE);
        }
        VOLT_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete!");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void COpenGLFramebuffer::DeleteFramebuffers()
    {
        glDeleteFramebuffers(1, &m_rendererID);
        glDeleteTextures(static_cast<uint32_t>(m_colorAttachments.size()), m_colorAttachments.data());
        glDeleteTextures(1, &m_depthAttachment);

        m_colorAttachments.clear();
        m_depthAttachment = 0;
    }
}
