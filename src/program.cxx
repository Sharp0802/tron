#include <filesystem>
#include <iostream>

#include "program.h"
#include "shader.h"

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
}
