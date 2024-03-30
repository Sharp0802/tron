#ifndef TRON_MESH_H
#define TRON_MESH_H

#include "oop/oop.h"
#include "vertexarray.h"
#include "vertexbuffer.h"

namespace tron::oop::specials
{
    /*
    enum class Primitive
    {
        Cube
    };
    */

    class Mesh
    {
        /*
         * DO NOT CHANGE ORDER OF FIELDS.
         *
         * VertexArray must be initialized after other VertexBuffers already initialized
         * Because VertexArray requires bindings from other VertexBuffers.
         */
        IndexBuffer m_ebo;
        VertexBuffer m_vbo;
        VertexArray m_vao;

    public:
        Mesh(
            std::initializer_list<VertexAttributeInfo> attributes,
            VertexBufferUsage usage = VertexBufferUsage::DEFAULT_DRAW);

        void Bind();

        void Draw();

        // [[nodiscard]]
        // static Mesh GetPrimitiveObject(Primitive type);

        immutable_(IndexBuffer*, EBO);
        immutable_(VertexBuffer*, VBO);
    };
}

#endif //TRON_MESH_H
