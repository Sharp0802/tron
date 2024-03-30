#include "pch.h"
#include "log.h"
#include "oop/specials/texture2d.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace
{
    GLuint GenTexture2D()
    {
        GLuint ret;
        glGenTextures(1, &ret);
        return ret;
    }

    tron::TextureFormat GetDefaultFormat(int nChannel)
    {
        switch (nChannel)
        {
        case 1:
            return tron::TextureFormat::R;
        case 2:
            return tron::TextureFormat::RG;
        case 3:
            return tron::TextureFormat::RGB;
        case 4:
            return tron::TextureFormat::RGBA;

        default:
            throw std::range_error("Channel number must be in [1-4].");
        }
    }
}

namespace tron::oop::specials
{
    Texture2D::Texture2D(
        const char* img,
        const TextureWrap wrap,
        const TextureFilter filter,
        const TextureFormat format,
        const TextureType type)
        : Texture2D(std::string(img), wrap, filter, format, type)
    {
    }

    Texture2D::Texture2D(
        const std::string& img,
        TextureWrap wrap,
        TextureFilter filter,
        TextureFormat format,
        TextureType type)
        : m_handle(GenTexture2D()), m_disposed(false)
    {
        int width, height, nChannel;
        auto* data = stbi_load(img.data(), &width, &height, &nChannel, 0);
        if (!data)
        {
            std::stringstream ss;
            ss << "Failed to load texture from image '" << img << "'.";
            std::string str(ss.str());
            log::stbi.err() << str << std::endl;
            throw std::runtime_error(str);
        }

        glBindTexture(GL_TEXTURE_2D, m_handle);

        auto glWrap = static_cast<GLint>(wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glWrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glWrap);

        auto glFilter = static_cast<GLint>(filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFilter);

        auto glFormat = static_cast<GLenum>(
            format == decltype(format)::DEFAULT ? GetDefaultFormat(nChannel) : format);
        auto glInternalFormat = static_cast<GLint>(glFormat);
        auto glType           = static_cast<GLenum>(type);

        glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, width, height, 0, glFormat, glType, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }

    Texture2D::~Texture2D()
    {
        if (m_handle)
            glDeleteTextures(1, &m_handle);
        m_disposed = true;
    }

    void Texture2D::Bind()
    {
        if (m_disposed)
            throw std::runtime_error("An object already disposed.");
        glActiveTexture(m_handle);
        glBindTexture(GL_TEXTURE_2D, m_handle);
    }
}
