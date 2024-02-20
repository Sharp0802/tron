#ifndef EXCEPT_H
#define EXCEPT_H

#include <system_error>
#include <GL/glew.h>

#include "stdexcept.h"

struct gl_error final : public std::system_error
{
    gl_error()
        : std::system_error(glGetError(), gl_category())
    {
    }

    explicit gl_error(const std::string& what)
        : std::system_error(glGetError(), gl_category(), what)
    {
    }
};

#endif //EXCEPT_H
