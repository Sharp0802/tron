#ifndef WINDOW_H
#define WINDOW_H

#include "oop/oop.h"
#include "oop/cobject.h"

namespace tron::oop::sys
{
    class Window : public CObject
    {
        friend class Input;

        GLFWwindow* m_window;
        int         m_width;
        int         m_height;
        std::string m_title;

        void UpdateFPSCounter();

        void OnScroll(double x, double y) const;

        void OnMouseMove(double x, double y) const;

    public:
        Window();

        void Bind();

        void Draw();

        void SetInputMode(int key, int mod);

        immutable_(int, Width);
        immutable_(int, Height);
        mutable_(const char*, Title);
        mutable_(bool, ShouldClose);

        static bool Instanced();

        static Window& Instance();
    };
}

#endif //WINDOW_H
