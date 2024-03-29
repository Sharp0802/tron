#ifndef TRON_VERTEXBUFFER_H
#define TRON_VERTEXBUFFER_H

#include "pch.h"

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
		STREAM_DRAW = GL_STREAM_DRAW,

		DEFAULT_DRAW = STATIC_DRAW
	};

	class VertexBuffer
	{
		const VertexBufferTarget m_target;
		const VertexBufferUsage m_usage;
		const GLenum m_binding;
		const GLuint m_handle;

		void EnsureBinding() const;

	protected:
		bool m_disposed;

	public:
		VertexBuffer& operator=(const VertexBuffer&) = delete;
		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer() = delete;

		explicit VertexBuffer(VertexBufferTarget target, VertexBufferUsage usage = VertexBufferUsage::DEFAULT_DRAW);

		virtual ~VertexBuffer();

		void Bind();

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
		size_t m_size;

	public:
		explicit IndexBuffer(VertexBufferUsage usage);

		using VertexBuffer::Bind;
		using VertexBuffer::Target;
		using VertexBuffer::Usage;
		using VertexBuffer::BufferSubData;

		void Buffer(std::initializer_list<uint8_t> buffer);

		void Buffer(std::initializer_list<uint16_t> buffer);

		void Buffer(std::initializer_list<uint32_t> buffer);

		[[nodiscard]]
		GLenum Type() const;

		[[nodiscard]]
		size_t Size() const;
	};
}


#endif //TRON_VERTEXBUFFER_H
