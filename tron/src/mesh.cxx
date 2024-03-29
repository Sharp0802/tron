#include "pch.h"
#include "mesh.h"

namespace tron
{
    Mesh::Mesh(std::initializer_list<VertexAttributeInfo> attributes, VertexBufferUsage usage)
        : m_ebo(usage),
          m_vbo(VertexBufferTarget::ARRAY_BUFFER, usage),
          m_vao(attributes)
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

    ptr<Mesh> Mesh::GetPrimitiveObject(Primitive type)
    {
        switch (type)
        {
        case Primitive::Cube:
        {

#define UNIT 1.0f

            float points[] = {
                -UNIT, UNIT, UNIT, 0, 1, 0, 1, 1,
                UNIT, UNIT, UNIT, 0, 0, 1, 1, 0,
                UNIT, -UNIT, UNIT, 1, 0, 0, 0, 0,
                -UNIT, -UNIT, UNIT, 1, 0, 0, 0, 1,

                UNIT, UNIT, UNIT, 0, 1, 0, 1, 1,
                UNIT, UNIT, -UNIT, 0, 0, 1, 1, 0,
                UNIT, -UNIT, -UNIT, 1, 0, 0, 0, 0,
                UNIT, -UNIT, UNIT, 1, 0, 0, 0, 1,

                UNIT, UNIT, -UNIT, 0, 1, 0, 1, 1,
                -UNIT, UNIT, -UNIT, 0, 0, 1, 1, 0,
                -UNIT, -UNIT, -UNIT, 1, 0, 0, 0, 0,
                UNIT, -UNIT, -UNIT, 1, 0, 0, 0, 1,

                -UNIT, UNIT, -UNIT, 0, 1, 0, 1, 1,
                -UNIT, UNIT, UNIT, 0, 0, 1, 1, 0,
                -UNIT, -UNIT, UNIT, 1, 0, 0, 0, 0,
                -UNIT, -UNIT, -UNIT, 1, 0, 0, 0, 1,

                -UNIT, UNIT, -UNIT, 0, 1, 0, 1, 1,
                UNIT, UNIT, -UNIT, 0, 0, 1, 1, 0,
                UNIT, UNIT, UNIT, 1, 0, 0, 0, 0,
                -UNIT, UNIT, UNIT, 1, 0, 0, 0, 1,

                -UNIT, -UNIT, UNIT, 0, 1, 0, 1, 1,
                UNIT, -UNIT, UNIT, 0, 0, 1, 1, 0,
                UNIT, -UNIT, -UNIT, 1, 0, 0, 0, 0,
                -UNIT, -UNIT, -UNIT, 1, 0, 0, 0, 1,
            };

#define RECT_P(p, i) (p+(i*4))
#define RECT(i) RECT_P(0,i),RECT_P(1,i),RECT_P(2,i),RECT_P(0,i),RECT_P(2,i),RECT_P(3,i)

            std::initializer_list<uint16_t> indices = {
                RECT(0), RECT(1), RECT(2), RECT(3), RECT(4), RECT(5),
            };

            auto mesh = std::make_shared_mesh({
                    VertexAttributeInfo::Create<glm::vec3>(),
                    VertexAttributeInfo::Create<glm::vec3>(),
                    VertexAttributeInfo::Create<glm::vec2>(),
            });
            mesh->Bind();
            mesh->VBO.Buffer(points, sizeof points);
            mesh->EBO.Buffer(indices);

            return mesh;
        }
        default:
            throw std::range_error("Undefined primitive mesh type");
        }
    }
}
