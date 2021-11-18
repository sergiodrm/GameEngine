#pragma once


#include "VoltEngine/Renderer/Texture.h"

namespace Volt
{
    class COpenGLTexture2D : public ITexture
    {
    public:
        virtual ~COpenGLTexture2D() override;

        /** Begin ITexture interface */
        virtual void Bind(uint32_t slot = 0) const override;
        virtual void Unbind() const override;

        virtual uint32_t GetRendererID() const override { return m_rendererID; }

        virtual uint32_t GetWidth() const override { return m_width; }
        virtual uint32_t GetHeight() const override { return m_height; }
        virtual void SetWidth(uint32_t width) override { m_width = width; }
        virtual void SetHeight(uint32_t height) override { m_height = height; }

        virtual void SetData(void* data, uint32_t size) override;

        virtual bool operator==(const ITexture& other) const override;
        /** End ITexture interface */

        /** Begin IResource interface */
        virtual void Load(const std::string& filepath) override;
        virtual void Unload() override;
        /** End IResource interface */

    protected:
        void LoadFromFile();
        void LoadProceduralTexture();

    private:
        uint32_t m_rendererID {0};
        uint32_t m_width {0};
        uint32_t m_height {0};
        uint32_t m_internalFormat {0};
        uint32_t m_dataFormat {0};
    };
}
