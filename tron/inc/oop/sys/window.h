#ifndef WINDOW_H
#define WINDOW_H

#include "oop/oop.h"
#include "oop/cobject.h"

namespace tron::oop::sys
{
    class Window : public CObject
    {
        GLFWwindow* m_window;
        int         m_width;
        int         m_height;
        std::string m_title;

        void (*m_onScroll)(float v) = nullptr;
        void (*m_onMouseMove)(float x, float y) = nullptr;

        void UpdateFPSCounter();

        void OnScroll(double x, double y) const;

        void OnMouseMove(double x, double y) const;

    public:
        Window();

        void Bind();

        void Draw();

        immutable_(int, Width);
        immutable_(int, Height);
        mutable_(const char*, Title);
        mutable_(bool, ShouldClose);

        static_immutable_(bool, Instanced);

        static Window& Instance();
    };
}

#endif //WINDOW_H
