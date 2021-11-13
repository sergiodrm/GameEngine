#pragma once


#include "VoltEngine/Renderer/Texture.h"

namespace Volt
{
    class COpenGLTexture2D : public ITexture
    {
    public:
        COpenGLTexture2D(uint32_t width, uint32_t height);
        COpenGLTexture2D(const std::string& filepath);
        virtual ~COpenGLTexture2D() override;

        virtual void Bind(uint32_t slot = 0) const override;
        virtual void Unbind() const override;

        virtual uint32_t GetRendererID() const override { return m_rendererID; }

        virtual uint32_t GetWidth() const override { return m_width; }
        virtual uint32_t GetHeight() const override { return m_height; }

        virtual void SetData(void* data, uint32_t size) override;

        virtual bool operator==(const ITexture& other) const override;
    private:
        uint32_t m_rendererID;
        uint32_t m_width;
        uint32_t m_height;
        uint32_t m_internalFormat;
        uint32_t m_dataFormat;
    };
}
