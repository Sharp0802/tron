#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include "resource.h"

#include <unordered_map>

namespace tron
{
	namespace shader_detail
	{
		template<typename T>
		void Set(GLint loc, T value)
		{
			static_assert(false, "template parameter T is not primitive for OpenGL");
		}

		template<>
		void Set<int>(GLint loc, int value);

		template<>
		void Set<float>(GLint loc, float value);

		template<>
		void Set<glm::vec2>(GLint loc, glm::vec2 value);

		template<>
		void Set<glm::vec3>(GLint loc, glm::vec3 value);

		template<>
		void Set<glm::vec4>(GLint loc, glm::vec4 value);

		template<>
		void Set<glm::mat2>(GLint loc, glm::mat2 value);

		template<>
		void Set<glm::mat3>(GLint loc, glm::mat3 value);

		template<>
		void Set<glm::mat4>(GLint loc, glm::mat4 value);
	}

    class Shader : protected Resource
    {
        const GLenum m_type;
        const GLuint m_handle;

    public:
        explicit Shader(const std::string& name);

        [[nodiscard]]
        GLuint GetHandle() const;

		void PrintLog() const;

		static void PrintLog(GLuint handle);

		template<typename T>
		void Set(GLint loc, T value)
		{
			return shader_detail::Set<T>(loc, value);
		}
    };
}

#endif //SHADER_H
