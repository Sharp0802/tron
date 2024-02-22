#include <GL/glew.h>
#include <sstream>

#include "log.h"

namespace tron
{
	void log::LogGLParameters()
	{
		constexpr GLenum params[] = {
				GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
				GL_MAX_CUBE_MAP_TEXTURE_SIZE,
				GL_MAX_DRAW_BUFFERS,
				GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
				GL_MAX_TEXTURE_IMAGE_UNITS,
				GL_MAX_TEXTURE_SIZE,
				GL_MAX_VARYING_FLOATS,
				GL_MAX_VERTEX_ATTRIBS,
				GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
				GL_MAX_VERTEX_UNIFORM_COMPONENTS,
				GL_MAX_VIEWPORT_DIMS,
				GL_STEREO,
		};
		constexpr const char* names[] = {
				"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
				"GL_MAX_CUBE_MAP_TEXTURE_SIZE",
				"GL_MAX_DRAW_BUFFERS",
				"GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
				"GL_MAX_TEXTURE_IMAGE_UNITS",
				"GL_MAX_TEXTURE_SIZE",
				"GL_MAX_VARYING_FLOATS",
				"GL_MAX_VERTEX_ATTRIBS",
				"GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
				"GL_MAX_VERTEX_UNIFORM_COMPONENTS",
				"GL_MAX_VIEWPORT_DIMS",
				"GL_STEREO",
		};

		std::stringstream ss;

		ss << "<GL CONTEXT PARAMETERS DUMP>" << std::endl
		   << "================================" << std::endl;

		for (GLint i = 0, v = 0; i < 10; ++i)
		{
			glGetIntegerv(params[i], &v);
			ss << names[i] << '=' << v << std::endl;
		}

		GLint v[2];
		v[0] = v[1] = 0;
		glGetIntegerv(params[10], v);
		ss << names[10] << "=(" << v[0] << ',' << v[1] << ')' << std::endl;

		GLboolean s = 0;
		glGetBooleanv(params[11], &s);
		ss << names[11] << '=' << static_cast<GLint>(s) << std::endl;

		ss << "================================" << std::endl;

		gl.out() << ss.str();
	}
}
