#ifndef TRON_TEXTURE_H
#define TRON_TEXTURE_H

#include "pch.h"

namespace tron
{
	enum class TextureWrap : GLint
	{
		REPEAT               = GL_REPEAT,
		CLAMP_TO_EDGE        = GL_CLAMP_TO_EDGE,
		CLAMP_TO_BORDER      = GL_CLAMP_TO_BORDER,
		MIRRORED_REPEAT      = GL_MIRRORED_REPEAT,
		MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE,

		DEFAULT = REPEAT
	};

	enum class TextureFilter : GLint
	{
		NEAREST                = GL_NEAREST,
		NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
		NEAREST_MIPMAP_LINEAR  = GL_NEAREST_MIPMAP_LINEAR,
		LINEAR                 = GL_LINEAR,
		LINEAR_MIPMAP_NEAREST  = GL_LINEAR_MIPMAP_NEAREST,
		LINEAR_MIPMAP_LINEAR   = GL_LINEAR_MIPMAP_LINEAR,

		DEFAULT = NEAREST
	};

	enum class TextureFormat : GLenum
	{
		R               = GL_RED,
		RG              = GL_RG,
		RGB             = GL_RGB,
		RGBA            = GL_RGBA,
		BGR             = GL_BGR,
		BGRA            = GL_BGRA,
		STENCIL_INDEX   = GL_STENCIL_INDEX,
		DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
		DEPTH_STENCIL   = GL_DEPTH_STENCIL,

		DEFAULT = std::numeric_limits<GLenum>::max()
	};

	enum class TextureType : GLenum
	{
		UNSIGNED_BYTE               = GL_UNSIGNED_BYTE,
		BYTE                        = GL_BYTE,
		UNSIGNED_SHORT              = GL_UNSIGNED_SHORT,
		SHORT                       = GL_SHORT,
		UNSIGNED_INT                = GL_UNSIGNED_INT,
		INT                         = GL_INT,
		HALF_FLOAT                  = GL_HALF_FLOAT,
		FLOAT                       = GL_FLOAT,
		UNSIGNED_BYTE_3_2_2         = GL_UNSIGNED_BYTE_3_3_2,
		UNSIGNED_BYTE_2_3_3_REV     = GL_UNSIGNED_BYTE_2_3_3_REV,
		UNSIGNED_SHORT_5_6_5        = GL_UNSIGNED_SHORT_5_6_5,
		UNSIGNED_SHORT_5_6_5_REV    = GL_UNSIGNED_SHORT_5_6_5_REV,
		UNSIGNED_SHORT_4_4_4_4      = GL_UNSIGNED_SHORT_4_4_4_4,
		UNSIGNED_SHORT_4_4_4_4_REV  = GL_UNSIGNED_SHORT_4_4_4_4_REV,
		UNSIGNED_SHORT_5_5_5_1      = GL_UNSIGNED_SHORT_5_5_5_1,
		UNSIGNED_SHORT_1_5_5_5_REV  = GL_UNSIGNED_SHORT_1_5_5_5_REV,
		UNSIGNED_INT_8_8_8_8        = GL_UNSIGNED_INT_8_8_8_8,
		UNSIGNED_INT_8_8_8_8_REV    = GL_UNSIGNED_INT_8_8_8_8_REV,
		UNSIGNED_INT_10_10_10_2     = GL_UNSIGNED_INT_10_10_10_2,
		UNSIGNED_INT_2_10_10_10_REV = GL_UNSIGNED_INT_2_10_10_10_REV,

		DEFAULT = UNSIGNED_BYTE
	};
	
	class Texture
	{
	public:
	    virtual ~Texture() = default;
	
	    virtual void Bind() = 0;
	};
}

#endif //TRON_TEXTURE_H
