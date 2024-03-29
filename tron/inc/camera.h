#ifndef CAMERA_H
#define CAMERA_H

#include "pch.h"
#include "transform.h"
#include "def.h"

namespace tron
{
    class Camera
    {
        ptr<Transform> m_transform;

        [[nodiscard]]
        glm::vec3 Rotate(glm::vec3 v) const;

    public:
        Camera();

        [[nodiscard]]
        ptr<Transform> get_Transform() const;

        [[nodiscard]]
        glm::mat4 get_Matrix() const;

        __declspec(property(get=get_Transform))
        ptr<Transform> Transform;

        __declspec(property(get=get_Matrix))
        glm::mat4 Matrix;
    };
}

#endif
