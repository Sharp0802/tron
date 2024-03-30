#include "oop/cobject.h"

namespace tron::oop
{
    void CObject::OnCreated()
    {
        if (m_events.Created)
            m_events.Created(this);
    }

    void CObject::OnEnabled()
    {
        if (m_events.Enabled)
            m_events.Enabled(this);
    }

    void CObject::OnUpdated(float delta)
    {
        if (m_events.Updated)
            m_events.Updated(this, delta);
    }

    void CObject::OnDisabled()
    {
        if (m_events.Disabled)
            m_events.Disabled(this);
    }

    void CObject::OnDestroyed()
    {
        if (m_events.Destroyed)
            m_events.Destroyed(this);
    }

    void CObject::AssertAccess() const
    {
        if (m_disposing)
            throw std::runtime_error("access to disposed object");
    }

    CObject::CObject(TypeId type, CObjectEvents events)
        : Type(type),
          m_events(events),
          m_disposing(false),
          m_disposed(false),
          m_lastActive(false),
          m_active(false)
    {
        m_events.Created(this);
        m_active = true;
    }

    void CObject::Update(float delta)
    {
        AssertAccess();

        if (m_lastActive != m_active)
        {
            (m_active ? m_events.Enabled : m_events.Disabled)(this);
            m_lastActive = m_active;
        }

        m_events.Updated(this, delta);

        if (m_disposing && !m_disposed)
        {
            m_events.Destroyed(this);
            m_disposed = true;
        }
    }

    void CObject::Destroy()
    {
        m_disposing = true;
    }
}
