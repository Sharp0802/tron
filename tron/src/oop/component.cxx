#include "oop/component.h"

namespace tron::oop
{
    Component::Component(TypeId type, Actor* parent, CObjectEvents events) // NOLINT(*-pro-type-member-init)
        : CObject(type, events),
          Parent(parent)
    {
    }
}
