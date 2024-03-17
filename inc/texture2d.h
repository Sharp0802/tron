#ifndef TRON_TEXTURE2D_H
#define TRON_TEXTURE2D_H

#include "pch.h"
#include "texture.h"

namespace tron
{

	class Texture2D final : public Texture
	{
		const GLuint m_handle;

		bool m_disposed;

	public:
		explicit Texture2D(
				const std::string& img,
				TextureWrap wrap = TextureWrap::DEFAULT,
				TextureFilter filter = TextureFilter::DEFAULT,
				TextureFormat format = TextureFormat::DEFAULT,
				TextureType type = TextureType::DEFAULT);

		~Texture2D() override;

		void Bind() override;
	};

}

#endif //TRON_TEXTURE2D_H
