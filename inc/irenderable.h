#ifndef TRON_IRENDERABLE_H
#define TRON_IRENDERABLE_H

#include "pch.h"

namespace tron
{

	class IRenderable
	{
	public:
		virtual ~IRenderable() = default;

		virtual void Draw() = 0;
	};

}

#endif //TRON_IRENDERABLE_H
