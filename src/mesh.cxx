#include <system_error>
#include "mesh.h"

namespace tron
{
	Mesh::Mesh(std::initializer_list<VertexAttributeInfo> attributes, VertexBufferUsage usage)
		: m_vao(attributes),
		  m_vbo(VertexBufferTarget::ARRAY_BUFFER, usage),
		  m_ebo(usage)
	{
	}

	VertexBuffer& Mesh::get_VBO()
	{
		return m_vbo;
	}

	IndexBuffer& Mesh::get_EBO()
	{
		return m_ebo;
	}

	void Mesh::Bind()
	{
		m_vbo.Bind();
		m_ebo.Bind();
	}

	void Mesh::Draw()
	{
		m_vao.Bind();

		auto size = m_ebo.Size();
		if (size > std::numeric_limits<GLsizei>::max())
			throw std::system_error(E2BIG, std::generic_category(), "Indices length is too big to draw");

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(size), m_ebo.Type(), nullptr);
	}
}
