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

		GLuint SwapBinding();

	public:
		VertexBuffer& operator=(const VertexBuffer&) = delete;
		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer() = delete;

		explicit VertexBuffer(VertexBufferTarget target, VertexBufferUsage usage = VertexBufferUsage::STATIC_DRAW);

		~VertexBuffer();

		void Bind();

		__attribute__((access(read_only, 2, 3)))
		void Buffer(void* buffer, size_t size);

		void BufferSubData(void* buffer, size_t offset, size_t size);

		[[nodiscard]]
		VertexBufferTarget Target() const;

		[[nodiscard]]
		VertexBufferUsage Usage() const;
	};

	class IndexBuffer : protected VertexBuffer
	{
		GLenum m_type;

	public:
		void Buffer(const std::vector<uint8_t>& buffer);

		void Buffer(const std::vector<uint16_t>& buffer);

		void Buffer(const std::vector<uint32_t>& buffer);
	};
}


#endif //TRON_VERTEXBUFFER_H
