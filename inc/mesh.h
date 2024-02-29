#ifndef TRON_MESH_H
#define TRON_MESH_H

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "irenderable.h"

namespace tron
{

	class Mesh : public IRenderable
	{
		VertexArray  m_vao;
		VertexBuffer m_vbo;
		IndexBuffer  m_ebo;

	public:
		Mesh(
				std::initializer_list<VertexAttributeInfo> attributes,
				VertexBufferUsage usage = VertexBufferUsage::DEFAULT_DRAW);

		[[nodiscard]]
		VertexBuffer& VBO();

		[[nodiscard]]
		IndexBuffer& EBO();

		void Draw() override;
	};

} // tron

#endif //TRON_MESH_H
