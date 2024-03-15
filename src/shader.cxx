#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

#include "sys/memorymappedfile.h"
#include "gl/except.h"
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

        const sys::MemoryMappedFile mmf(m_name);
        const auto                  addr = mmf.GetAddress();

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
			PrintLog();
			throw gl_error(msg);
		}
    }

    GLuint Shader::GetHandle() const
    {
        return m_handle;
    }

	void Shader::PrintLog() const
	{
		PrintLog(m_handle);
	}

	void Shader::PrintLog(GLuint handle)
	{
		static __thread char buf[BUFSIZ];
		int cchMax    = BUFSIZ;
		int cchActual = 0;

		glGetShaderInfoLog(handle, cchMax, &cchActual, buf);
		log::gl.trc() << "<SHADER INFORMATION [" << handle << "]>" << std::endl
		              << buf << std::endl;
	}

#if ASSERT_SUCCEED_SET__FIXED
#define ASSERT_SUCCEED_SET                                             \
	if (auto err = glGetError(); err != GL_NO_ERROR)                   \
    {                                                                  \
        log::gl.err() << "Couldn't pass data to " << loc << std::endl; \
        throw gl_error(err, "Couldn't pass data");                     \
    }
#else
#define ASSERT_SUCCEED_SET __asm__("nop")
#endif

	template<>
	void shader_detail::Set<int>(GLint loc, int value)
	{
		glUniform1i(loc, value);
		ASSERT_SUCCEED_SET;
	}

	template<>
	void shader_detail::Set<float>(GLint loc, float value)
	{
		glUniform1f(loc, value);
		ASSERT_SUCCEED_SET;
	}

	template<>
	void shader_detail::Set<glm::vec2>(GLint loc, glm::vec2 value)
	{
		glUniform2f(loc, value.x, value.y);
		ASSERT_SUCCEED_SET;
	}

	template<>
	void shader_detail::Set<glm::vec3>(GLint loc, glm::vec3 value)
	{
		glUniform3f(loc, value.x, value.y, value.z);
		ASSERT_SUCCEED_SET;
	}

	template<>
	void shader_detail::Set<glm::vec4>(GLint loc, glm::vec4 value)
	{
		glUniform4f(loc, value.x, value.y, value.z, value.w);
		ASSERT_SUCCEED_SET;
	}

	template<>
	void shader_detail::Set<glm::mat2>(GLint loc, glm::mat2 value)
	{
		glUniformMatrix2fv(loc, 1, GL_FALSE, glm::value_ptr(value));
		ASSERT_SUCCEED_SET;
	}

	template<>
	void shader_detail::Set<glm::mat3>(GLint loc, glm::mat3 value)
	{
		glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
		ASSERT_SUCCEED_SET;
	}

	template<>
	void shader_detail::Set<glm::mat4>(GLint loc, glm::mat4 value)
	{
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
		ASSERT_SUCCEED_SET;
	}
}
