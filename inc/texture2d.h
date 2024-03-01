#ifndef TRON_TEXTURE2D_H
#define TRON_TEXTURE2D_H

#include <GL/glew.h>
#include <string>

namespace tron
{

	class Texture2D
	{
		const GLuint m_handle;

		bool m_disposed;

	public:
		Texture2D(const std::string& img);

		~Texture2D();

		void Bind();
	};

}

#endif //TRON_TEXTURE2D_H
