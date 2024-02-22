#ifndef TRON_PROGRAM_H
#define TRON_PROGRAM_H

#include <GL/glew.h>
#include <string>
#include <vector>
#include <memory>

#include "shader.h"

namespace tron
{
	class Program
	{
		GLuint m_handle;
		bool   m_isUsing;

		std::vector<std::shared_ptr<Shader>> m_shaders;

	public:
		Program();

		~Program();

		void Use();

		void AttachResourceRoot(const std::string& root);

		void Validate() const;

		void PrintLog() const;

		void PrintAll() const;

		static void PrintLog(GLuint handle);

		static void PrintAll(GLuint handle);
	};
}

#endif //TRON_PROGRAM_H
