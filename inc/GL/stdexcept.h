#ifndef STDEXCEPT_H
#define STDEXCEPT_H

#include <system_error>
#include <GL/glew.h>

struct gl_category final : std::error_category
{
    const char* name() const noexcept override
    {
        return "gl_category";
    }

    std::string message(const int code) const override
    {
        return reinterpret_cast<const char*>(glewGetErrorString(code));
    }
};

#endif //STDEXCEPT_H
