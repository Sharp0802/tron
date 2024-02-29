#include "vertexbuffer.h"
#include "def.h"

namespace
{
	GLuint CreateBuffer()
	{
		GLuint ret;
		glGenBuffers(1, &ret);
		return ret;
	}

	GLenum GetBindingEnum(tron::VertexBufferTarget target)
	{
#ifndef VBO_BINDING
#define VBO_BINDING(t) { tron::VertexBufferTarget:: t , GL_##t##_BINDING }
#endif
		static std::unordered_map<tron::VertexBufferTarget, GLenum> bindingMap = {
				VBO_BINDING(ARRAY_BUFFER),
				VBO_BINDING(ELEMENT_ARRAY_BUFFER),
		};

		return bindingMap[target];
	}
}

namespace tron
{
	VertexBuffer::VertexBuffer(VertexBufferTarget target, VertexBufferUsage usage)
			: m_target(target),
			  m_usage(usage),
			  m_binding(GetBindingEnum(target)),
			  m_handle(CreateBuffer()),
			  m_disposed(false)
	{
	}

#define ASSERT_ALIVE ASSERT(!m_disposed)

	GLuint VertexBuffer::SwapBinding()
	{
		ASSERT_ALIVE;

		GLuint old;
		glGetIntegerv(m_binding, reinterpret_cast<GLint*>(&old));

		Bind();

		return old;
	}

	void VertexBuffer::Bind()
	{
		ASSERT_ALIVE;

		glBindBuffer(static_cast<GLenum>(m_target), m_handle);
	}

	void VertexBuffer::Buffer(void* buffer, size_t size)
	{
		ASSERT_ALIVE;

		/*
		 * DO NOT BELIEVE DEVELOPERS TO CALL Buffer() AFTER CALLING SwapBinding().
		 * glBufferData is not context-free. Context must be load/restore-ed.
		 */
		auto old = SwapBinding();

		glBufferData(
				static_cast<GLenum>(m_target),
				static_cast<ssize_t>(size),
				buffer,
				static_cast<GLenum>(m_usage));

		/*
		 * Restore context
		 *
		 * If this is the first VBO, error will be generated on glBindBuffer().
		 * Just ignore it.
		 */
		if (old != m_handle)
			glBindBuffer(static_cast<GLenum>(m_target), old);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_handle);
		m_disposed = true;
	}

	void VertexBuffer::BufferSubData(void* buffer, size_t offset, size_t size)
	{
		ASSERT_ALIVE;

		/*
		 * DO NOT BELIEVE DEVELOPERS TO CALL Buffer() AFTER CALLING SwapBinding().
		 * glBufferData is not context-free. Context must be load/restore-ed.
		 */
		auto old = SwapBinding();

		glBufferSubData(
				static_cast<GLenum>(m_target),
				static_cast<ssize_t>(offset),
				static_cast<ssize_t>(size),
				buffer);

		/*
		 * Restore context
		 *
		 * If this is the first VBO, error will be generated on glBindBuffer().
		 * Just ignore it.
		 */
		if (old != m_handle)
			glBindBuffer(static_cast<GLenum>(m_target), old);
	}

	VertexBufferTarget VertexBuffer::Target() const
	{
		ASSERT_ALIVE;
		return m_target;
	}

	VertexBufferUsage VertexBuffer::Usage() const
	{
		ASSERT_ALIVE;
		return m_usage;
	}

	IndexBuffer::IndexBuffer(VertexBufferUsage usage)
		: VertexBuffer(VertexBufferTarget::ELEMENT_ARRAY_BUFFER, usage),
		  m_type(GL_INVALID_ENUM),
		  m_size(0)
	{
	}

	void IndexBuffer::Buffer(const std::vector<uint8_t>& buffer)
	{
		ASSERT_ALIVE;

		VertexBuffer::Buffer(
				(void*)buffer.data(),
				buffer.size() * sizeof(uint8_t));
		m_type = GL_UNSIGNED_BYTE;
		m_size = buffer.size();
	}

	void IndexBuffer::Buffer(const std::vector<uint16_t>& buffer)
	{
		ASSERT_ALIVE;

		VertexBuffer::Buffer(
				(void*)buffer.data(),
				buffer.size() * sizeof(uint16_t));
		m_type = GL_UNSIGNED_SHORT;
		m_size = buffer.size();
	}

	void IndexBuffer::Buffer(const std::vector<uint32_t>& buffer)
	{
		ASSERT_ALIVE;

		VertexBuffer::Buffer(
				(void*)buffer.data(),
				buffer.size() * sizeof(uint32_t));
		m_type = GL_UNSIGNED_INT;
		m_size = buffer.size();
	}

	GLenum IndexBuffer::Type() const
	{
		ASSERT_ALIVE;
		return m_type;
	}

	size_t IndexBuffer::Size() const
	{
		ASSERT_ALIVE;
		return m_size;
	}
}
