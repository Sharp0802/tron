#include "texture2d.h"
#include "log.h"

#include <sstream>
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
}

namespace tron
{
	Texture2D::Texture2D(const std::string& img)
		: m_handle(GenTexture2D()), m_disposed(false)
	{
		int width, height, nrChannels;
		auto* data = stbi_load(img.data(), &width, &height, &nrChannels, 0);
		if (!data)
		{
			std::stringstream ss;
			ss << "Failed to load texture from image '" << img << "'.";
			std::string str(ss.str());
			log::stbi.err() << str << std::endl;
			throw std::runtime_error(str);
		}

		glBindTexture(GL_TEXTURE_2D, m_handle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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
