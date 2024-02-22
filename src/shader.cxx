#include <sstream>
#include "shader.h"

#include "memorymappedfile.h"
#include "GL/except.h"
#include "log.h"

namespace
{
	GLenum GetShaderType(const std::string& name)
	{
		if (name.size() < 7 ||
			!name.ends_with(".glsl") ||
			name[name.size() - 7] != '.')
			throw std::range_error("invalid shader name: '" + name + "'");

		switch (name[name.size() - 6])
		{
		case 'v': return GL_VERTEX_SHADER;
		case 'f': return GL_FRAGMENT_SHADER;
		default: throw std::range_error("invalid shader type: '" + name + "'");
		}
	}

    std::string GetShaderPath(const std::string& name)
    {
        return "sha/" + name;
    }
}

namespace tron
{
    Shader::Shader(const std::string& name)
        : Resource(GetShaderPath(name)),
          m_type(GetShaderType(m_name)),
          m_handle(glCreateShader(m_type))
    {
        if (!m_handle)
            throw gl_error();
        if (m_handle == GL_INVALID_ENUM)
            throw gl_error("`type` is not an accepted value");

        const MemoryMappedFile mmf(m_name);
        const auto             addr = mmf.GetAddress();

        glShaderSource(m_handle, 1, reinterpret_cast<const GLchar* const*>(&addr), nullptr);
        glCompileShader(m_handle);

		GLint params = -1;
		glGetShaderiv(m_handle, GL_COMPILE_STATUS, &params);
		if (GL_TRUE != params)
		{
			std::stringstream ss;
			ss << "Couldn't compile GL shader [" << m_handle << "]";
			std::string msg(ss.str());

			log::gl.err() << msg << std::endl;
			LogShaderInfo();
			throw gl_error(msg);
		}
    }

    GLuint Shader::GetHandle() const
    {
        return m_handle;
    }

	void Shader::LogShaderInfo() const
	{
		LogShaderInfo(m_handle);
	}

	void Shader::LogShaderInfo(GLuint handle)
	{
		__thread static char buf[BUFSIZ];
		int cchMax    = BUFSIZ;
		int cchActual = 0;

		glGetShaderInfoLog(handle, cchMax, &cchActual, buf);
		log::gl.trc() << "<SHADER INFORMATION [" << handle << "]>" << std::endl
		              << buf << std::endl;
	}
}
