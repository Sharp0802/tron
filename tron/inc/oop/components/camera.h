#ifndef CAMERA_H
#define CAMERA_H

#include "oop/oop.h"
#include "oop/component.h"

namespace tron::oop::components
{
    class Camera final : public Component
    {
        glm::mat4 m_projection{};
        glm::mat4 m_view{};
        float     m_fov = 45.f;

        float m_near = .1f;
        float m_far  = 100.f;

    public:
        explicit Camera(Actor* parent);

        mutable_(float, Near);
        mutable_(float, Far);
        mutable_(float, FoV);
        immutable_(glm::mat4, Projection);
        immutable_(glm::mat4, View);
    };
}

#endif //CAMERA_H
