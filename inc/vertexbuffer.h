#ifndef TRON_VERTEXBUFFER_H
#define TRON_VERTEXBUFFER_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <cstdint>

namespace tron
{
	enum class VertexBufferTarget : GLenum
	{
		ARRAY_BUFFER = GL_ARRAY_BUFFER,
		ELEMENT_ARRAY_BUFFER = GL_ELEMENT_ARRAY_BUFFER
	};

	enum class VertexBufferUsage : GLenum
	{
		STATIC_DRAW = GL_STATIC_DRAW,
		DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
		STREAM_DRAW = GL_STREAM_DRAW
	};

	class VertexBuffer
	{
		const VertexBufferTarget m_target;
		const VertexBufferUsage m_usage;
		const GLenum m_binding;
		const GLuint m_handle;

		bool m_disposed;

	public:
		explicit VertexBuffer(VertexBufferTarget target, VertexBufferUsage usage = VertexBufferUsage::STATIC_DRAW);

		~VertexBuffer();

		GLuint Bind();

		__attribute__((access(read_only, 1, 2)))
		void Buffer(void* buffer, size_t size);

		void BufferSubData(void* buffer, size_t offset, size_t size);

		[[nodiscard]]
		VertexBufferTarget Target() const;

		[[nodiscard]]
		VertexBufferUsage Usage() const;
	};
}


#endif //TRON_VERTEXBUFFER_H
