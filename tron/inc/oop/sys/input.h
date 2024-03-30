#ifndef INPUT_H
#define INPUT_H

#include "oop/oop.h"

namespace tron::oop::sys
{
    class Input
    {
    public:
        static bool GetKey(int key);

        static void Poll();
    };
}

#endif //INPUT_H
