#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "oop/oop.h"
#include "oop/component.h"

namespace tron::oop::components
{
    class Transform final : public Component
    {
        Transform*              m_parent;
        std::vector<Transform*> m_children;

        glm::vec3 m_position;
        glm::quat m_rotation;
        glm::vec3 m_scale;

    public:
        explicit Transform(Actor* actor, Transform* parent);

        [[nodiscard]]
        glm::vec3 ToParent(glm::vec3 v) const;

        [[nodiscard]]
        uint32_t CountChild() const;

        void AddChild(Transform* child);

        [[nodiscard]]
        Transform* GetChild(uint32_t i) const;


        mutable_(Transform*, Parent);

        mutable_(glm::vec3, Position);

        mutable_(glm::vec3, LocalPosition);

        mutable_(glm::quat, Rotation);

        mutable_(glm::vec3, Scale);

        immutable_(glm::mat4, Matrix);
    };
}

#endif //TRANSFORM_H
