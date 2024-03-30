#ifndef COMPONENT_H
#define COMPONENT_H

#include "oop.h"
#include "cobject.h"

namespace tron::oop
{
    class Actor;

    class Component : public CObject
    {
    public:
        Actor* Parent;

        Component(TypeId type, Actor* parent, CObjectEvents events);
    };
} // tron

#endif //COMPONENT_H
