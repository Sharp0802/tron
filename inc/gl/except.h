#ifndef EXCEPT_H
#define EXCEPT_H

#include <system_error>
#include <GL/glew.h>

#include "stdexcept.h"

struct gl_error final : public std::system_error
{
	explicit gl_error(GLenum error)
		: std::system_error(static_cast<int>(error), gl_category())
	{
	}

	gl_error()
		: gl_error(glGetError())
	{
	}

	explicit gl_error(GLenum error, const std::string& what)
		: std::system_error(static_cast<int>(error), gl_category(), what)
	{
	}

    explicit gl_error(const std::string& what)
		: gl_error(glGetError(), what)
    {
    }
};

#endif //EXCEPT_H
