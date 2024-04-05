#ifndef ACTOR_H
#define ACTOR_H

#include "oop.h"
#include "cobject.h"
#include "component.h"

namespace tron::oop
{
    class Actor final : public CObject
    {
        std::vector<Component*> m_components;

    protected:
        void OnCreated() override;
        void OnEnabled() override;
        void OnUpdated(float delta) override;
        void OnDisabled() override;
        void OnDestroyed() override;

    public:
        explicit Actor(CObjectEvents events);
        ~Actor() override;

        [[nodiscard]]
        int CountComponent(TypeId type) const;

        Component* GetComponent(TypeId type, int i = 0);

        void AddComponent(Component* component);

        bool RemoveComponent(Component* component);
    };
}

#endif //ACTOR_H
