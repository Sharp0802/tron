#include "oop/sys/input.h"

#include <oop/sys/window.h>

namespace tron::oop::sys
{
    static glm::vec2 s_move;
    static float s_scroll;

    void Input::SetMouseMove(glm::vec2 move)
    {
        s_move = move;
    }

    void Input::SetScroll(float y)
    {
        s_scroll = y;
    }

    bool Input::GetKey(const int key)
    {
        if (!Window::Instanced())
            return false;
        return glfwGetKey(Window::Instance().m_window, key);
    }

    void Input::Poll()
    {
        s_scroll = 0;
        glfwPollEvents();
    }

    glm::vec2 Input::GetMouseMove()
    {
        return s_move;
    }

    bool Input::GetMouseClick(const int button)
    {
        if (!Window::Instanced())
            return false;
        return glfwGetMouseButton(Window::Instance().m_window, button);
    }

    float Input::GetScroll()
    {
        return s_scroll;
    }
}
