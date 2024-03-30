#include "oop/specials/vertexarray.h"

namespace
{
    GLuint CreateVertexArray()
    {
        GLuint ret;
        glGenVertexArrays(1, &ret);
        return ret;
    }
}

namespace tron::oop::specials
{
    VertexArray::VertexArray(std::initializer_list<VertexAttributeInfo> attributes)
        : m_attributes(attributes),
          m_handle(CreateVertexArray()),
          m_disposed(false)
    {
        glBindVertexArray(m_handle);

        GLint vbo;
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vbo);
        if (vbo == GL_NONE)
            throw std::runtime_error("the binding to GL_ARRAY_BUFFER must not be null to initialize VertexArray");

        size_t stride = 0;
        for (const auto& attribute: m_attributes)
            stride += attribute.Size;

        for (size_t i = 0, offset = 0; i < m_attributes.size(); ++i)
        {
            auto attribute = m_attributes[i];
            glVertexAttribPointer(
                i,
                attribute.Count,
                attribute.Type,
                GL_FALSE,
                static_cast<GLsizei>(stride),
                reinterpret_cast<const void*>(offset));
            glEnableVertexAttribArray(i);

            offset += attribute.Size;
        }
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_handle);
        m_disposed = true;
    }

    void VertexArray::Bind()
    {
        if (m_disposed)
            throw std::runtime_error("An object already disposed.");
        glBindVertexArray(m_handle);
    }
}
