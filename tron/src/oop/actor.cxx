#include "oop/actor.h"
#include "oop/components/transform.h"

namespace tron::oop
{
    void Actor::OnCreated()
    {
        for (const auto component: m_components)
            component->OnCreated();
        CObject::OnCreated();
    }

    void Actor::OnEnabled()
    {
        for (const auto component: m_components)
            component->OnEnabled();
        CObject::OnEnabled();
    }

    void Actor::OnUpdated(float delta)
    {
        for (const auto component: m_components)
            component->OnUpdated(delta);
        CObject::OnUpdated(delta);
    }

    void Actor::OnDisabled()
    {
        for (const auto component: m_components)
            component->OnDisabled();
        CObject::OnDisabled();
    }

    void Actor::OnDestroyed()
    {
        for (const auto component: m_components)
            component->OnDestroyed();
        CObject::OnDestroyed();
    }

    Actor::Actor(CObjectEvents events): CObject(GetType<Actor>(), events)
    {
    }

    Actor::~Actor()
    {
        for (const auto component: m_components)
            delete component;
    }

    int Actor::CountComponent(TypeId type) const
    {
        int c = 0;
        for (const auto& component: m_components)
            if (component->Type == type)
                c++;
        return c;
    }

    Component* Actor::GetComponent(TypeId type, int i)
    {
        for (const auto& component: m_components)
            if (component->Type == type && i-- == 0)
                return component;
        return nullptr;
    }

    void Actor::AddComponent(Component* component)
    {
        m_components.push_back(component);
    }

    bool Actor::RemoveComponent(Component* component)
    {
        component->Destroy();
        for (auto v = m_components.cbegin(); v != m_components.cend(); ++v)
        {
            if (*v == component)
            {
                m_components.erase(v);
                return true;
            }
        }
        return false;
    }
}
