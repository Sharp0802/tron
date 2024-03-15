#include <filesystem>
#include <iostream>

#include "program.h"
#include "shader.h"
#include "log.h"
#include "gl/except.h"

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

	void Program::Link()
	{
		glLinkProgram(m_handle);
		GLenum err = glGetError();

		GLint params = -1;
		glGetProgramiv(m_handle, GL_LINK_STATUS, &params);
		if (params != GL_TRUE)
		{
			std::stringstream ss;
			ss << "Couldn't link GL shader program [" << m_handle << ']';
			std::string msg(ss.str());

			log::gl.err() << msg << std::endl;
			PrintLog();
			throw gl_error(err, msg);
		}
	}

	void Program::Use()
	{
		glUseProgram(m_handle);
		m_isUsing = true;
	}

	void Program::Validate() const
	{
		glValidateProgram(m_handle);

		GLint params = -1;
		glGetProgramiv(m_handle, GL_VALIDATE_STATUS, &params);
		(GL_TRUE == params ? log::gl.out() : log::gl.err())
			<< "program [" << m_handle << "] GL_VALIDATE_STATUS = " << params << std::endl;
		if (GL_TRUE == params)
			return;

		PrintLog();
		throw gl_error(GL_NO_ERROR, "Shader program validation failed.");
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
		static __thread char buf[BUFSIZ];
		int cchMax    = BUFSIZ;
		int cchActual = 0;

		glGetProgramInfoLog(handle, cchMax, &cchActual, buf);
		log::gl.trc() << "<SHADER PROGRAM LOG [" << handle << "]>" << std::endl
					  << buf << std::endl
					  << "========================" << std::endl;
	}

	void Program::PrintAll(GLuint handle)
	{
		static __thread char name[64];
		static __thread char extName[64];
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

		log::gl.trc() << ss.str() << "========================" << std::endl;

		PrintLog(handle);
	}

	GLint Program::GetLocation(const std::string& name)
	{
		if (auto iter = m_loc.find(name); iter != m_loc.end())
			return iter->second;

		auto loc = glGetUniformLocation(m_handle, name.data());
		if (loc != -1)
			return loc;

		log::gl.err() << "There is no uniform variable named as '" << name << "'" << std::endl;
		throw gl_error("Couldn't retrieve location of uniform variable.");
	}

	Shader& Program::AttachShader(const std::string& name)
	{
		auto shared = std::make_shared<Shader>(name);
		glAttachShader(m_handle, shared->GetHandle());
		m_shaders.push_back(shared);
		return *shared;
	}

	void Program::BindAttribute(GLuint location, const std::string& name)
	{
		glBindAttribLocation(m_handle, location, name.data());
		if (GLenum err = glGetError(); err != GL_NO_ERROR)
		{
			log::gl.err() << "Couldn't bind attribute location (" << name << " >>> " << location << ")" << std::endl;
			throw gl_error(err, "Couldn't bind attribute location");
		}
	}
}
