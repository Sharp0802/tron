#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include "resource.h"

namespace tron
{
    class Shader : protected Resource
    {
        const GLenum m_type;
        const GLuint m_handle;

    public:
        explicit Shader(const std::string& name);

        [[nodiscard]]
        GLuint GetHandle() const;
    };
}

#endif //SHADER_H
