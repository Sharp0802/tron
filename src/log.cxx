#include "pch.h"
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

		gl.trc() << ss.str();
	}

	std::string log::GLTypeToString(GLenum type)
	{
		switch(type)
		{
		case GL_BOOL:              return "bool";
		case GL_INT:               return "int";
		case GL_FLOAT:             return "float";
		case GL_FLOAT_VEC2:        return "vec2";
		case GL_FLOAT_VEC3:        return "vec3";
		case GL_FLOAT_VEC4:        return "vec4";
		case GL_FLOAT_MAT2:        return "mat2";
		case GL_FLOAT_MAT3:        return "mat3";
		case GL_FLOAT_MAT4:        return "mat4";
		case GL_SAMPLER_2D:        return "sampler2D";
		case GL_SAMPLER_3D:        return "sampler3D";
		case GL_SAMPLER_CUBE:      return "samplerCube";
		case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
		default:                   return "unknown";
		}
	}
}
