#ifndef TRON_VERTEXARRAY_H
#define TRON_VERTEXARRAY_H

#include <cstdint>
#include <GL/glew.h>

#include <glm/glm.hpp>

#include "renderer.h"

namespace tron
{
	struct VertexAttributeInfo
	{
		GLenum Type;
		GLint  Size;
		GLint  Count;
	};

	template<typename T>
	struct VertexAttribute
	{
		constexpr static GLint  Size  = sizeof(T);
		constexpr static GLenum Type  = -1;
		constexpr static GLint  Count = -1;

		constexpr static VertexAttributeInfo Create()
		{
			static_assert(Type != -1 && Count != -1, "Type `T` is not primitive");
			return { Type, Size, Count };
		}
	};

	template<>
	struct VertexAttribute<int>
	{
		constexpr static GLenum Type  = GL_INT;
		constexpr static GLint  Count = 1;
	};

	template<>
	struct VertexAttribute<float>
	{
		constexpr static GLenum Type  = GL_FLOAT;
		constexpr static GLint  Count = 1;
	};

#ifndef VERTEX_ATTRIBUTE_INFO_FROM_VEC_N
#define VERTEX_ATTRIBUTE_INFO_FROM_VEC_N(n)                      \
		template<>                                               \
		struct VertexAttribute<glm::vec##n>                      \
		{                                                        \
			constexpr static GLenum Type  = GL_FLOAT;            \
			constexpr static GLint  Count = n;                   \
		}
#endif

#ifndef VERTEX_ATTRIBUTE_INFO_FROM_MAT_N
#define VERTEX_ATTRIBUTE_INFO_FROM_MAT_N(n)                      \
		template<>                                               \
		struct VertexAttribute<glm::mat##n>                      \
		{                                                        \
			constexpr static GLenum Type  = GL_FLOAT;            \
			constexpr static GLint  Count = n*n;                 \
		}
#endif

	VERTEX_ATTRIBUTE_INFO_FROM_VEC_N(1);
	VERTEX_ATTRIBUTE_INFO_FROM_VEC_N(2);
	VERTEX_ATTRIBUTE_INFO_FROM_VEC_N(3);
	VERTEX_ATTRIBUTE_INFO_FROM_VEC_N(4);

	VERTEX_ATTRIBUTE_INFO_FROM_MAT_N(2);
	VERTEX_ATTRIBUTE_INFO_FROM_MAT_N(3);
	VERTEX_ATTRIBUTE_INFO_FROM_MAT_N(4);

	class VertexArray
	{
		using Attributes = std::vector<VertexAttributeInfo>;

		const Attributes m_attributes;
		const GLuint     m_handle;

		bool m_disposed;

		GLuint SwapBinding();

	public:
		VertexArray(std::initializer_list<VertexAttributeInfo> attributes);

		~VertexArray();

		void Bind();
	};
}

#endif //TRON_VERTEXARRAY_H
