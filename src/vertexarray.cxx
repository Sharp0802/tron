#include "vertexarray.h"
#include "def.h"

namespace
{
	GLuint CreateVertexArray()
	{
		GLuint ret;
		glGenVertexArrays(1, &ret);
		return ret;
	}
}

namespace tron
{
	GLuint VertexArray::SwapBinding()
	{
		GLuint old;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, reinterpret_cast<GLint*>(&old));

		Bind();

		return old;
	}

	VertexArray::VertexArray(std::initializer_list<VertexAttributeInfo> attributes)
		: m_attributes(attributes),
		  m_handle(CreateVertexArray()),
		  m_disposed(false)
	{
		auto old = SwapBinding();

		size_t stride = 0;
		for (const auto& attribute: m_attributes)
			stride += attribute.Size;

		for (size_t i = 0; i < m_attributes.size(); ++i)
		{
			auto attribute = m_attributes[i];
			glVertexAttribPointer(
					i,
					attribute.Count,
					attribute.Type,
					GL_FALSE,
					attribute.Size,
					reinterpret_cast<const void*>(stride));
			glEnableVertexAttribArray(i);
		}

		/*
		 * Restore context
		 *
		 * If this is the first VAO, error will be generated on glBindVertexArray().
		 * Just ignore it.
		 */
		glBindVertexArray(old);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_handle);
		m_disposed = true;
	}

	void VertexArray::Bind()
	{
		ASSERT(!m_disposed);
		glBindVertexArray(m_handle);
	}
}
