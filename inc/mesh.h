#ifndef TRON_MESH_H
#define TRON_MESH_H

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "irenderable.h"

namespace tron
{

	class Mesh : public IRenderable
	{
		/*
		 * DO NOT CHANGE ORDER OF FIELDS.
		 *
		 * VertexArray must be initialized after other VertexBuffers already initialized
		 * Because VertexArray requires bindings from other VertexBuffers.
		 */
		IndexBuffer  m_ebo;
		VertexBuffer m_vbo;
		VertexArray  m_vao;

	public:
		Mesh(
				std::initializer_list<VertexAttributeInfo> attributes,
				VertexBufferUsage usage = VertexBufferUsage::DEFAULT_DRAW);

		[[nodiscard]]
		VertexBuffer& VBO();

		[[nodiscard]]
		IndexBuffer& EBO();

		void Bind();

		void Draw() override;
	};

} // tron

#endif //TRON_MESH_H
