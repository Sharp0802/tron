#ifndef INPUT_H
#define INPUT_H

#include "oop/oop.h"

namespace tron::oop::sys
{
    class Input
    {
        friend class Window;

        static void SetMouseMove(glm::vec2 move);

        static void SetScroll(float y);

    public:
        static bool GetKey(int key);

        static void Poll();

        static glm::vec2 GetMouseMove();

        static float GetScroll();
    };
}

#endif //INPUT_H
