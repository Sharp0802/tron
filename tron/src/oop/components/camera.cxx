#include "oop/components/camera.h"

#include <oop/components/transform.h>

#include "oop/actor.h"
#include "oop/sys/window.h"

namespace tron::oop::components
{
    static Camera* m_current;

    class Transform;

    Camera::Camera(Actor* parent): Component(GetType<Camera>(), parent, {
        .Updated = [](CObject* object, float)
        {
            if (!sys::Window::Instanced())
                return;

            auto* camera = dynamic_cast<Camera*>(object);

            // Update projection matrix
            camera->m_projection = glm::perspective(
                glm::radians(camera->m_fov),
                static_cast<float>(sys::Window::Instance().Width) / static_cast<float>(sys::Window::Instance().Height),
                camera->m_near,
                camera->m_far);

            // Update view matrix
            const auto transform = dynamic_cast<Transform*>(camera->Parent->GetComponent(GetType<Transform>()));

            const auto origin = transform->Position;
            const auto forward = transform->ToParent({0, 0, 1});
            const auto up = transform->ToParent({0, 1, 0});

            camera->m_view = lookAt(origin, origin + forward, up);
        }
    })
    {
    }

    void Camera::Bind()
    {
        m_current = this;
    }

    getter_decl_(Camera, Near)
    {
        return m_near;
    }

    setter_decl_(Camera, Near)
    {
        m_near = value;
    }

    getter_decl_(Camera, Far)
    {
        return m_far;
    }

    setter_decl_(Camera, Far)
    {
        m_far = value;
    }

    getter_decl_(Camera, FoV)
    {
        return m_fov;
    }

    setter_decl_(Camera, FoV)
    {
        m_fov = value;
    }

    getter_decl_(Camera, Projection)
    {
        return m_projection;
    }

    getter_decl_(Camera, View)
    {
        return m_view;
    }

    Camera& Camera::Current()
    {
        return *m_current;
    }
}
