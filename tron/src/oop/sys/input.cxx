#include "oop/sys/input.h"

#include <oop/sys/window.h>

namespace tron::oop::sys
{
    bool Input::GetKey(const int key)
    {
        if (!Window::Instanced())
            return false;
        return glfwGetKey(Window::Instance().m_window, key);
    }

    void Input::Poll()
    {
        glfwPollEvents();
    }
}
