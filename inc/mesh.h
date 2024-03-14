#ifndef TRON_MESH_H
#define TRON_MESH_H

#include <vector>
#include <unordered_map>
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
		VertexBuffer& get_VBO();

		[[nodiscard]]
		IndexBuffer& get_EBO();

		void Bind();

		void Draw() override;

		__declspec(property(get=get_VBO))
		VertexBuffer& VBO;

		__declspec(property(get=get_EBO))
		IndexBuffer& EBO;
	};

} // tron

#endif //TRON_MESH_H
