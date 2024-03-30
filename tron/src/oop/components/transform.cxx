#include "oop/components/transform.h"

namespace tron::oop::components
{
    Transform::Transform(Actor* actor, Transform* parent)
        : Component(GetType<Transform>(), actor, {}),
          m_parent(parent),
          m_position(0),
          m_rotation(),
          m_scale(1)
    {
    }

    glm::vec3 Transform::ToParent(glm::vec3 v) const
    {
        v = glm::rotate(v, Rotation.x, {1, 0, 0});
        v = glm::rotate(v, Rotation.y, {0, 1, 0});
        v = glm::rotate(v, Rotation.z, {0, 0, 1});
        return v;
    }

    uint32_t Transform::CountChild() const
    {
        return m_children.size();
    }

    void Transform::AddChild(Transform* child)
    {
        m_children.push_back(child);
    }

    Transform* Transform::GetChild(uint32_t i) const
    {
        return m_children[i];
    }

    getter_decl_(Transform, Position)
    {
        return m_position;
    }

    setter_decl_(Transform, Position)
    {
        m_position = value;
    }

    getter_decl_(Transform, LocalPosition)
    {
        return { 0, 0, 0 };
    }

    setter_decl_(Transform, LocalPosition)
    {
        Position += ToParent(value);
    }

    getter_decl_(Transform, Rotation)
    {
        return m_rotation;
    }

    setter_decl_(Transform, Rotation)
    {
        m_rotation = value;
    }

    getter_decl_(Transform, Scale)
    {
        return m_scale;
    }

    setter_decl_(Transform, Scale)
    {
        m_scale = value;
    }

    getter_decl_(Transform, Matrix)
    {
        const glm::mat4 t = translate(glm::mat4(1.0), m_position);
        const glm::mat4 r = mat4_cast(m_rotation);
        const glm::mat4 s = scale(glm::mat4(1.0), m_scale);

        return t * r * s;
    }
}
