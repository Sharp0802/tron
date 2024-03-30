#ifndef ACTOR_H
#define ACTOR_H

#include "oop.h"
#include "cobject.h"
#include "component.h"

namespace tron::oop
{
    class Actor final : public CObject
    {
        std::map<TypeId, Component*> m_componentTable;

    protected:
        void OnCreated() override;
        void OnEnabled() override;
        void OnUpdated(float delta) override;
        void OnDisabled() override;
        void OnDestroyed() override;

    public:
        explicit Actor(CObjectEvents events);
        ~Actor() override;

        Component* GetComponent(TypeId type);

        bool TryAddComponent(Component* component);

        void RemoveComponent(Component* component);
    };
}

#endif //ACTOR_H
