#include "pch.h"
#include "camera.h"

namespace tron
{
    glm::vec3 Camera::Rotate(glm::vec3 v) const
    {
        v = rotate(v, Transform->Rotation.x, glm::vec3(1, 0, 0));
        v = rotate(v, Transform->Rotation.y, glm::vec3(0, 1, 0));
        v = rotate(v, Transform->Rotation.z, glm::vec3(0, 0, 1));
        return v;
    }

    Camera::Camera()
        : m_transform(std::make_shared<class Transform>())
    {
    }

    ptr<Transform> Camera::get_Transform() const
    {
        return m_transform;
    }

    glm::mat4 Camera::get_Matrix() const
    {
        const auto origin = Transform->Position;
        const auto forward = Transform->ToParent({0, 0, 1});
        const auto up = Transform->ToParent({0, 1, 0});
        return lookAt(origin, origin + forward, up);
    }
}
