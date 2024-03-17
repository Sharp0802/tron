#ifndef TRON_MESH_H
#define TRON_MESH_H

#include "pch.h"
#include "vertexarray.h"
#include "vertexbuffer.h"
#include "irenderable.h"
#include "def.h"

namespace tron
{
    enum class Primitive
    {
        Cube
    };

    class Mesh : public IRenderable
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

        [[nodiscard]]
        VertexBuffer& get_VBO();

        [[nodiscard]]
        IndexBuffer& get_EBO();

        void Bind();

        void Draw() override;

        [[nodiscard]]
        static ptr<Mesh> GetPrimitiveObject(Primitive type);

        __declspec(property(get=get_VBO))
        VertexBuffer& VBO;

        __declspec(property(get=get_EBO))
        IndexBuffer& EBO;
    };
}

namespace std
{
    using namespace tron;

    inline shared_ptr<_NonArray<Mesh>>
    make_shared_mesh(
        std::initializer_list<VertexAttributeInfo>&& attributes)
    {
        return std::make_shared<Mesh>(attributes);
    }

    inline shared_ptr<_NonArray<Mesh>>
    make_shared_mesh(
        std::initializer_list<VertexAttributeInfo>&& attributes,
        VertexBufferUsage&& usage)
    {
        return std::make_shared<Mesh>(attributes, usage);
    }
}

#endif //TRON_MESH_H
