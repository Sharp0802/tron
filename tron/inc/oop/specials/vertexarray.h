#ifndef TRON_VERTEXARRAY_H
#define TRON_VERTEXARRAY_H

#include "oop/oop.h"

namespace tron::oop::specials
{
    template<typename T>
    struct VertexAttribute;

    template<>
    struct VertexAttribute<int>
    {
        constexpr static GLenum Type  = GL_INT;
        constexpr static GLint  Size  = sizeof(int);
        constexpr static GLint  Count = 1;
    };

    template<>
    struct VertexAttribute<float>
    {
        constexpr static GLenum Type  = GL_FLOAT;
        constexpr static GLint  Size  = sizeof(float);
        constexpr static GLint  Count = 1;
    };

#ifndef VERTEX_ATTRIBUTE_INFO_FROM_VEC_N
#define VERTEX_ATTRIBUTE_INFO_FROM_VEC_N(n)                      \
		template<>                                               \
		struct VertexAttribute<glm::vec##n>                      \
		{                                                        \
			constexpr static GLenum Type  = GL_FLOAT;            \
			constexpr static GLint  Size  = sizeof(glm::vec##n); \
			constexpr static GLint  Count = n;                   \
		}
#endif

#ifndef VERTEX_ATTRIBUTE_INFO_FROM_MAT_N
#define VERTEX_ATTRIBUTE_INFO_FROM_MAT_N(n)                      \
		template<>                                               \
		struct VertexAttribute<glm::mat##n>                      \
		{                                                        \
			constexpr static GLenum Type  = GL_FLOAT;            \
			constexpr static GLint  Size  = sizeof(glm::mat##n); \
			constexpr static GLint  Count = n*n;                 \
		}
#endif

    VERTEX_ATTRIBUTE_INFO_FROM_VEC_N(1);

    VERTEX_ATTRIBUTE_INFO_FROM_VEC_N(2);

    VERTEX_ATTRIBUTE_INFO_FROM_VEC_N(3);

    VERTEX_ATTRIBUTE_INFO_FROM_VEC_N(4);

    VERTEX_ATTRIBUTE_INFO_FROM_MAT_N(2);

    VERTEX_ATTRIBUTE_INFO_FROM_MAT_N(3);

    VERTEX_ATTRIBUTE_INFO_FROM_MAT_N(4);

    class VertexAttributeInfo
    {
    public:
        GLenum Type;
        GLint  Size;
        GLint  Count;

        template<typename T>
        constexpr static VertexAttributeInfo Create()
        {
            using A = VertexAttribute<T>;
            return { A::Type, A::Size, A::Count };
        }
    };

    class VertexArray
    {
        const std::vector<VertexAttributeInfo> m_attributes;
        const GLuint                           m_handle;

        bool m_disposed;

    public:
        VertexArray& operator=(const VertexArray&) = delete;
        VertexArray(const VertexArray&) = delete;
        VertexArray() = delete;

        VertexArray(std::initializer_list<VertexAttributeInfo> attributes);

        ~VertexArray();

        void Bind();
    };
}

#endif //TRON_VERTEXARRAY_H
