#include "vertexbuffer.h"

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
			  m_handle(CreateBuffer())
	{
	}

	GLuint VertexBuffer::Bind()
	{
		GLuint old;
		glGetIntegerv(m_binding, reinterpret_cast<GLint*>(&old));

		glBindBuffer(static_cast<GLenum>(m_target), m_handle);

		return old;
	}

	void VertexBuffer::Buffer(void* buffer, size_t size)
	{
		/*
		 * DO NOT BELIEVE DEVELOPERS TO CALL Buffer() AFTER CALLING Bind().
		 * glBufferData is not context-free. Context must be load/restore-ed.
		 */
		auto old = Bind();

		glBufferData(
				static_cast<GLenum>(m_target),
				static_cast<ssize_t>(size),
				buffer,
				static_cast<GLenum>(m_usage));

		/* Restore context */
		if (old != m_handle)
			glBindBuffer(static_cast<GLenum>(m_target), old);
	}

	void VertexBuffer::BufferSubData(void* buffer, size_t offset, size_t size)
	{
		/*
		 * DO NOT BELIEVE DEVELOPERS TO CALL Buffer() AFTER CALLING Bind().
		 * glBufferData is not context-free. Context must be load/restore-ed.
		 */
		auto old = Bind();

		glBufferSubData(
				static_cast<GLenum>(m_target),
				static_cast<ssize_t>(offset),
				static_cast<ssize_t>(size),
				buffer);

		/* Restore context */
		if (old != m_handle)
			glBindBuffer(static_cast<GLenum>(m_target), old);
	}

	VertexBufferTarget VertexBuffer::Target() const
	{
		return m_target;
	}

	VertexBufferUsage VertexBuffer::Usage() const
	{
		return m_usage;
	}
}
