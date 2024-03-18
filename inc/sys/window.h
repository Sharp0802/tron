#ifndef WINDOW_H
#define WINDOW_H

#include "pch.h"
#include "irenderable.h"

namespace tron::sys
{
    class Window : public IRenderable
    {
        GLFWwindow* m_window;
        int         m_width;
        int         m_height;
        std::string m_title;
        glm::mat4   m_projection;
        float       m_fov;

        void UpdateFPSCounter();

        void UpdateProjection();

        void OnScroll(double x, double y);

    public:
        Window(int w, int h, std::string title);

        void Bind();

        void Draw() override;

        void PollEvents();

        [[nodiscard]]
        bool GetKey(int key) const;

        [[nodiscard]]
        std::string get_Title() const;

        void put_Title(std::string title);

        [[nodiscard]]
        glm::mat4 get_Projection() const;

        [[nodiscard]]
        bool get_ShouldClose() const;

        void put_ShouldClose(bool value) const;

        __declspec(property(get=get_Title, put=put_Title))
        std::string Title;

        __declspec(property(get=get_Projection))
        glm::mat4 Projection;

        __declspec(property(get=get_ShouldClose, put=put_ShouldClose))
        bool ShouldClose;
    };
}

#endif
