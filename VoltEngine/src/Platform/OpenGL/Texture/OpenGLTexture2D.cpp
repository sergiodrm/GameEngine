#include "OpenGLTexture2D.h"

#include "OpenGLTexture2DLoader.h"
#include "stb_image.h"
#include "glad/glad.h"
#include "VoltEngine/Core/Time.h"

namespace Volt
{
    COpenGLTexture2D::~COpenGLTexture2D()
    {
        glDeleteTextures(1, &m_rendererID);
    }

    UniquePtr<IAssetLoader> COpenGLTexture2D::CreateLoader()
    {
        return CreateUnique<COpenGLTexture2DLoader>(this);
    }

    void COpenGLTexture2D::Bind(uint32_t slot) const
    {
        glBindTextureUnit(slot, m_rendererID);
    }

    void COpenGLTexture2D::Unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void COpenGLTexture2D::SetData(void* data, uint32_t size)
    {
        const uint32_t bytePerPixel = m_dataFormat == GL_RGBA ? 4 : 3;
        VOLT_ASSERT(size == m_width * m_height * bytePerPixel, "Data must fill the entire texture!");
        glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
    }

    bool COpenGLTexture2D::operator==(const ITexture& other) const
    {
        return m_rendererID == other.GetRendererID();
    }

    void COpenGLTexture2D::LoadData(const STexture2DAssetData& data)
    {
        PROFILE_SCOPE(Texture2DLoadData);
        m_width = static_cast<uint32_t>(data.Width);
        m_height = static_cast<uint32_t>(data.Height);

        m_internalFormat = data.Channels == 4 ? GL_RGBA8 : data.Channels == 3 ? GL_RGB8 : 0;
        m_dataFormat = data.Channels == 4 ? GL_RGBA : data.Channels == 3 ? GL_RGB : 0;
        VOLT_ASSERT(m_internalFormat & m_dataFormat, "Image format not supported: {0}", data.TextureName.c_str());

        glGenTextures(1, &m_rendererID);
        glBindTexture(GL_TEXTURE_2D, m_rendererID);
        glTextureStorage2D(m_rendererID, 1, m_internalFormat, m_width, m_height);

        glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data.Data);
        m_isLoaded = true;
    }
}
