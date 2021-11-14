#include "OpenGLTexture2D.h"

#include "stb_image.h"
#include "glad/glad.h"

namespace Volt
{
    COpenGLTexture2D::COpenGLTexture2D(IResourceManager* creator, const std::string& name, uint32_t id)
        : ITexture(creator, name, id) { }

    COpenGLTexture2D::~COpenGLTexture2D()
    { }

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

    void COpenGLTexture2D::Load()
    {
        switch (GetLoadType())
        {
            case ETextureLoadType::File: LoadFromFile();
                break;
            case ETextureLoadType::Procedural: LoadProceduralTexture();
                break;
        }
    }

    void COpenGLTexture2D::Unload()
    {
        glDeleteTextures(1, &m_rendererID);
    }

    void COpenGLTexture2D::LoadFromFile()
    {
        int32_t width;
        int32_t height;
        int32_t channels;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = stbi_load(GetName().c_str(), &width, &height, &channels, 0);
        VOLT_ASSERT(data, "Couldn't load image from: {0}", GetName().c_str());

        m_width = static_cast<uint32_t>(width);
        m_height = static_cast<uint32_t>(height);

        m_internalFormat = channels == 4 ? GL_RGBA8 : channels == 3 ? GL_RGB8 : 0;
        m_dataFormat = channels == 4 ? GL_RGBA : channels == 3 ? GL_RGB : 0;
        VOLT_ASSERT(m_internalFormat & m_dataFormat, "Image format not supported: {0}", GetName().c_str());

        glGenTextures(1, &m_rendererID);
        glBindTexture(GL_TEXTURE_2D, m_rendererID);
        glTextureStorage2D(m_rendererID, 1, m_internalFormat, m_width, m_height);

        glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    void COpenGLTexture2D::LoadProceduralTexture()
    {
        glGenTextures(1, &m_rendererID);

        m_internalFormat = GL_RGBA8;
        m_dataFormat = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, m_rendererID);
        glTextureStorage2D(m_rendererID, 1, m_internalFormat, m_width, m_height);

        glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
}
