#include "oop/actor.h"
#include "oop/components/transform.h"

namespace tron::oop
{
    void Actor::OnCreated()
    {
        for (const auto component: std::views::values(m_componentTable))
            component->OnCreated();
        CObject::OnCreated();
    }

    void Actor::OnEnabled()
    {
        for (const auto component: std::views::values(m_componentTable))
            component->OnEnabled();
        CObject::OnEnabled();
    }

    void Actor::OnUpdated(float delta)
    {
        for (const auto component: std::views::values(m_componentTable))
            component->OnUpdated(delta);
        CObject::OnUpdated(delta);
    }

    void Actor::OnDisabled()
    {
        for (const auto component: std::views::values(m_componentTable))
            component->OnDisabled();
        CObject::OnDisabled();
    }

    void Actor::OnDestroyed()
    {
        for (const auto component: std::views::values(m_componentTable))
            component->OnDestroyed();
        CObject::OnDestroyed();
    }

    Actor::Actor(CObjectEvents events): CObject(GetType<Actor>(), events)
    {
        TryAddComponent(new components::Transform(this, nullptr));
    }

    Actor::~Actor()
    {
        for (const auto component: std::views::values(m_componentTable))
            delete component;
    }

    Component* Actor::GetComponent(TypeId type)
    {
        return m_componentTable.contains(type) ? m_componentTable[type] : nullptr;
    }

    bool Actor::TryAddComponent(Component* component)
    {
        if (m_componentTable.contains(component->Type))
            return false;
        m_componentTable[component->Type] = component;
        return true;
    }

    void Actor::RemoveComponent(Component* component)
    {
        component->Destroy();
        m_componentTable.erase(component->Type);
    }
}
