#ifndef STDEXCEPT_H
#define STDEXCEPT_H

#include "pch.h"

struct gl_category final : std::error_category
{
	[[nodiscard]]
    const char* name() const noexcept override
    {
        return "gl_category";
    }

	[[nodiscard]]
    std::string message(const int code) const override
    {
		switch (code)
		{
		case GL_NO_ERROR:                      return "GL_NO_ERROR";
		case GL_INVALID_ENUM:                  return "GL_INVALID_ENUM";
		case GL_INVALID_VALUE:                 return "GL_INVALID_VALUE";
		case GL_INVALID_OPERATION:             return "GL_INVALID_OPERATION";
		case GL_STACK_OVERFLOW:                return "GL_STACK_OVERFLOW";
		case GL_STACK_UNDERFLOW:               return "GL_STACK_UNDERFLOW";
		case GL_OUT_OF_MEMORY:                 return "GL_OUT_OF_MEMORY";
		case GL_TABLE_TOO_LARGE:               return "GL_TABLE_TOO_LARGE";
		case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
		default:                               return "Unknown Error";
		}
    }
};

#endif //STDEXCEPT_H
