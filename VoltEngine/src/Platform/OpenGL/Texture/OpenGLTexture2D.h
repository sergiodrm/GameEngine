#pragma once


#include "VoltEngine/Renderer/Texture.h"

namespace Volt
{
    struct STexture2DAssetData;

    class COpenGLTexture2D : public ITexture
    {
    public:
        virtual ~COpenGLTexture2D() override;

        //~ IAsset methods
        virtual UniquePtr<IAssetLoader> CreateLoader() override;
        virtual bool IsLoaded() const override { return m_isLoaded; }
        //~ IAsset methods

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

        void LoadData(const STexture2DAssetData& data);

    private:
        bool m_isLoaded {false};
        uint32_t m_rendererID {0};
        uint32_t m_width {0};
        uint32_t m_height {0};
        uint32_t m_internalFormat {0};
        uint32_t m_dataFormat {0};
    };
}
