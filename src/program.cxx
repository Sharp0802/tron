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
			PrintLog();
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

	void Program::Validate() const
	{
		glValidateProgram(m_handle);
		GLenum err = glGetError();

		GLint params = -1;
		glGetProgramiv(m_handle, GL_VALIDATE_STATUS, &params);
		log::gl.out() << "program [" << m_handle << "] GL_VALIDATE_STATUS = " << params << std::endl;
		if (GL_TRUE == params)
			return;

		PrintLog();
		throw gl_error(err, "Shader program validation failed.");
	}

	void Program::PrintLog() const
	{
		PrintLog(m_handle);
	}

	void Program::PrintAll() const
	{
		PrintAll(m_handle);
	}

	void Program::PrintLog(GLuint handle)
	{
		__thread static char buf[BUFSIZ];
		int cchMax    = BUFSIZ;
		int cchActual = 0;

		glGetProgramInfoLog(handle, cchMax, &cchActual, buf);
		log::gl.trc() << "<SHADER PROGRAM LOG [" << handle << "]>" << std::endl
					  << buf << std::endl;
	}

	void Program::PrintAll(GLuint handle)
	{
		__thread static char name[64];
		__thread static char extName[64];
		GLint cchMax = sizeof name;

		std::stringstream ss;

		GLint params = -1;
		glGetProgramiv(handle, GL_LINK_STATUS, &params);
		ss << "      GL_LINK_STATUS = " << params << std::endl;

		glGetProgramiv(handle, GL_ATTACHED_SHADERS, &params);
		ss << " GL_ATTACHED_SHADERS = " << params << std::endl;

		glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTES, &params);
		ss << "GL_ACTIVE_ATTRIBUTES = " << params << std::endl;

		for (decltype(params) i = 0; i < params; ++i)
		{
			GLint  cchActual = 0;
			GLint  size      = 0;
			GLenum type      = GL_INVALID_ENUM;

			glGetActiveAttrib(handle, i, cchMax, &cchActual, &size, &type, name);
			if (size <= 1)
			{
				GLint location = glGetAttribLocation(handle, name);
				ss << '[' << i
				   << "] type:" << log::GLTypeToString(type)
				   << " name:" << name
				   << " location:" << location << std::endl;
			}
			else
			{
				for (decltype(size) j = 0; j < size; ++j)
				{
					sprintf(extName, "%s[%d]", name, j);
					GLint location = glGetAttribLocation(handle, extName);
					ss << '[' << i
					   << "] type:" << log::GLTypeToString(type)
					   << " name:" << extName
					   << " location:" << location << std::endl;
				}
			}
		}

		log::gl.trc() << ss.str();

		PrintLog(handle);
	}
}
