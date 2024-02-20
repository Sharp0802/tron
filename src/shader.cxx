#include "shader.h"

#include "memorymappedfile.h"
#include "GL/except.h"

namespace
{
    std::string GetShaderPath(const std::string& name)
    {
        return "sha/" + name + ".glsl";
    }
}

namespace tron
{
    Shader::Shader(const std::string& name, const GLenum type)
        : Resource(GetShaderPath(name)),
          m_type(type),
          m_handle(glCreateShader(type))
    {
        if (!m_handle)
            throw gl_error();
        if (m_handle == GL_INVALID_ENUM)
            throw gl_error("`type` is not an accepted value");

        const MemoryMappedFile mmf(m_name);
        const auto             addr = mmf.GetAddress();

        glShaderSource(m_handle, 1, reinterpret_cast<const GLchar* const*>(&addr), nullptr);
        glCompileShader(m_handle);
    }

    GLuint Shader::GetHandle() const
    {
        return m_handle;
    }
}
