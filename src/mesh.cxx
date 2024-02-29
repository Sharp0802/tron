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

	VertexBuffer& Mesh::VBO()
	{
		return m_vbo;
	}

	IndexBuffer& Mesh::EBO()
	{
		return m_ebo;
	}

	void Mesh::Draw()
	{
		m_vbo.Bind();
		m_ebo.Bind();
		m_vao.Bind();

		auto size = m_ebo.Size();
		if (size > std::numeric_limits<GLsizei>::max())
			throw std::system_error(E2BIG, std::generic_category());

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(size), m_ebo.Type(), nullptr);
	}
}
