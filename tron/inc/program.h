#ifndef TRON_PROGRAM_H
#define TRON_PROGRAM_H

#include "pch.h"
#include "shader.h"

namespace tron
{
	class Program
	{
		GLuint m_handle;
		bool   m_isUsing;

		std::vector<std::shared_ptr<Shader>>   m_shaders;
		std::unordered_map<std::string, GLint> m_loc;

	public:
		Program();

		~Program();

		void Link();

		void Use();

		GLint GetLocation(const std::string& name);

		Shader& AttachShader(const std::string& name);

		void BindAttribute(GLuint location, const std::string& name);

		void Validate() const;

		void PrintLog() const;

		void PrintAll() const;

		static void PrintLog(GLuint handle);

		static void PrintAll(GLuint handle);
	};
}

#endif //TRON_PROGRAM_H
