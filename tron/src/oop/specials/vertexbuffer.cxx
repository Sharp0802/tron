#include "pch.h"
#include "oop/specials/vertexbuffer.h"

namespace
{
    GLuint CreateBuffer()
    {
        GLuint ret;
        glGenBuffers(1, &ret);
        return ret;
    }

    GLenum GetBindingEnum(tron::oop::specials::VertexBufferTarget target)
    {
#ifndef VBO_BINDING
#define VBO_BINDING(t) { tron::oop::specials::VertexBufferTarget:: t , GL_##t##_BINDING }
#endif
        static std::unordered_map<tron::oop::specials::VertexBufferTarget, GLenum> bindingMap = {
            VBO_BINDING(ARRAY_BUFFER),
            VBO_BINDING(ELEMENT_ARRAY_BUFFER),
        };

        return bindingMap[target];
    }
}

namespace tron::oop::specials
{
    VertexBuffer::VertexBuffer(VertexBufferTarget target, VertexBufferUsage usage)
        : m_target(target),
          m_usage(usage),
          m_binding(GetBindingEnum(target)),
          m_handle(CreateBuffer()),
          m_disposed(false)
    {
        Bind();
    }

#define ASSERT_ALIVE { if (!m_disposed) throw std::runtime_error("An object already disposed."); }

    void VertexBuffer::EnsureBinding() const
    {
        ASSERT_ALIVE;

        GLuint old;
        glGetIntegerv(m_binding, reinterpret_cast<GLint*>(&old));
        if (old != m_handle)
            throw std::runtime_error("Invalid binding for vertex-buffer.");
    }

    void VertexBuffer::Bind()
    {
        ASSERT_ALIVE;

        glBindBuffer(static_cast<GLenum>(m_target), m_handle);
    }

    void VertexBuffer::Buffer(void* buffer, size_t size)
    {
        ASSERT_ALIVE;

        EnsureBinding();

        glBufferData(
            static_cast<GLenum>(m_target),
            static_cast<ssize_t>(size),
            buffer,
            static_cast<GLenum>(m_usage));
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &m_handle);
        m_disposed = true;
    }

    void VertexBuffer::BufferSubData(void* buffer, size_t offset, size_t size)
    {
        ASSERT_ALIVE;

        EnsureBinding();

        glBufferSubData(
            static_cast<GLenum>(m_target),
            static_cast<ssize_t>(offset),
            static_cast<ssize_t>(size),
            buffer);
    }

    VertexBufferTarget VertexBuffer::Target() const
    {
        ASSERT_ALIVE;
        return m_target;
    }

    VertexBufferUsage VertexBuffer::Usage() const
    {
        ASSERT_ALIVE;
        return m_usage;
    }

    IndexBuffer::IndexBuffer(VertexBufferUsage usage)
        : VertexBuffer(VertexBufferTarget::ELEMENT_ARRAY_BUFFER, usage),
          m_type(GL_INVALID_ENUM),
          m_size(0)
    {
    }

    void IndexBuffer::Buffer(std::initializer_list<uint8_t> buffer)
    {
        ASSERT_ALIVE;

        VertexBuffer::Buffer(
            const_cast<uint8_t*>(buffer.begin()),
            buffer.size() * sizeof(uint8_t));
        m_type = GL_UNSIGNED_BYTE;
        m_size = buffer.size();
    }

    void IndexBuffer::Buffer(std::initializer_list<uint16_t> buffer)
    {
        ASSERT_ALIVE;

        VertexBuffer::Buffer(
            const_cast<uint16_t*>(buffer.begin()),
            buffer.size() * sizeof(uint16_t));
        m_type = GL_UNSIGNED_SHORT;
        m_size = buffer.size();
    }

    void IndexBuffer::Buffer(std::initializer_list<uint32_t> buffer)
    {
        ASSERT_ALIVE;

        VertexBuffer::Buffer(
            const_cast<uint32_t*>(buffer.begin()),
            buffer.size() * sizeof(uint32_t));
        m_type = GL_UNSIGNED_INT;
        m_size = buffer.size();
    }

    GLenum IndexBuffer::Type() const
    {
        ASSERT_ALIVE;
        return m_type;
    }

    size_t IndexBuffer::Size() const
    {
        ASSERT_ALIVE;
        return m_size;
    }
}
