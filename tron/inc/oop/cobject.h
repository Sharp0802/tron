#ifndef COBJECT_H
#define COBJECT_H

#include "oop.h"

namespace tron::oop
{
    class CObject;

    struct CObjectEvents
    {
        template<typename... T>
        using Event = void(*)(CObject* self, T...);

        Event<>      Created;
        Event<>      Enabled;
        Event<float> Updated;
        Event<>      Disabled;
        Event<>      Destroyed;
    };

    class CObject
    {
        friend class Actor;

    public:
        const TypeId Type;

    private:
        CObjectEvents m_events;

        bool m_disposing;
        bool m_disposed;
        bool m_lastActive;
        bool m_active;

    protected:
        virtual void OnCreated();
        virtual void OnEnabled();
        virtual void OnUpdated(float delta);
        virtual void OnDisabled();
        virtual void OnDestroyed();

        void AssertAccess() const;

    public:
        CObject(TypeId type, CObjectEvents events);
        virtual ~CObject() = default;

        void Update(float delta);

        void Destroy();
    };
}

#endif //COBJECT_H
