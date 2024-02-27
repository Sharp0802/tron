#ifndef TRON_RENDERER_H
#define TRON_RENDERER_H

namespace tron
{

	class Renderer
	{
	public:
		virtual ~Renderer() = default;

		virtual void Draw() const = 0;
	};

}

#endif //TRON_RENDERER_H
