#include <filesystem>
#include <iostream>

#include "program.h"
#include "shader.h"
#include "log.h"
#include "GL/except.h"

namespace fs = std::filesystem;

namespace tron
{
	Program::Program()
			: m_handle(glCreateProgram()),
			  m_isUsing(false)
	{
	}

	Program::~Program()
	{
		glDeleteProgram(m_handle);
	}

	void Program::Use()
	{
		glLinkProgram(m_handle);

		GLint params = -1;
		glGetProgramiv(m_handle, GL_LINK_STATUS, &params);
		if (params != GL_TRUE)
		{
			std::stringstream ss;
			ss << "Couldn't link GL shader program [" << m_handle << ']';
			std::string msg(ss.str());

			log::gl.err() << msg << std::endl;
			LogProgramInfo();
			throw gl_error(msg);
		}

		glUseProgram(m_handle);
		m_isUsing = true;
	}

	void Program::AttachResourceRoot(const std::string& root)
	{
		if (m_isUsing)
			throw std::logic_error("program is already used by process");

		/*******************************************
		 * SHADERS                                 *
		 *******************************************/
		fs::path shaderRoot(root);
		shaderRoot += "/sha";
		for (const auto& entry : fs::recursive_directory_iterator(shaderRoot))
		{
			if (entry.is_directory())
				continue;

			const auto shaderId = fs::relative(entry.path(), shaderRoot).generic_string();
			const auto shader = std::make_shared<Shader>(shaderId);

			glAttachShader(m_handle, shader->GetHandle());

			m_shaders.push_back(shader);
		}
	}

	void Program::LogProgramInfo() const
	{
		LogProgramInfo(m_handle);
	}

	void Program::LogProgramInfo(GLuint handle)
	{
		__thread static char buf[BUFSIZ];
		int cchMax    = BUFSIZ;
		int cchActual = 0;

		glGetProgramInfoLog(handle, cchMax, &cchActual, buf);
		log::gl.trc() << "<SHADER PROGRAM INFORMATION [" << handle << "]>" << std::endl
					  << buf << std::endl;
	}
}
