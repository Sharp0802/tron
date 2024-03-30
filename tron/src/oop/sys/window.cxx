#include "oop/sys/window.h"
#include "log.h"

namespace tron::oop::sys
{
    static Window* s_window = nullptr;

    void Window::UpdateFPSCounter()
    {
        static double prev = glfwGetTime();
        static int    cFPS;
        static char   buf[BUFSIZ];

        const double current = glfwGetTime();

        if (const auto elapsed = current - prev; elapsed > 0.25)
        {
            prev = current;

            const auto fps = static_cast<double>(cFPS) / elapsed;

            snprintf(buf, sizeof buf, "%s @ fps: %.2lf", m_title.data(), fps);
            glfwSetWindowTitle(m_window, buf);
            cFPS = 0;
        }

        cFPS++;
    }

    void Window::OnScroll(double, const double y) const
    {
        if (m_onScroll)
            m_onScroll(static_cast<float>(y));
    }

    void Window::OnMouseMove(const double x, const double y) const
    {
        if (m_onMouseMove)
            m_onMouseMove(static_cast<float>(x), static_cast<float>(y));
    }

    Window::Window()
        : CObject(GetType<Window>(), {
            .Updated = [](CObject* self, float){
                dynamic_cast<Window*>(self)->UpdateFPSCounter();
            }
        }),
          m_window(nullptr),
          m_width(640),
          m_height(480)
    {
        if (s_window != nullptr)
            throw std::runtime_error("Couldn't handle multiple window");
        s_window = this;

        if (!glfwInit())
        {
            log::fw.err() << "glfwInit(): Couldn't start glfw3" << std::endl;
            throw std::runtime_error("Couldn't start glfw3");
        }

        /*
         * To support macOS, We should use OpenGL 3.2 core profile.
         * This let macOS use OpenGL 4.
         */
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_SAMPLES, 4);

        m_window = glfwCreateWindow(m_width, m_height, m_title.data(), nullptr, nullptr);
        if (!m_window)
        {
            log::fw.err() << "glfwCreateWindow(): Couldn't create window" << std::endl;
            glfwTerminate();
            throw std::runtime_error("Couldn't create window");
        }

        glfwSetWindowUserPointer(m_window, this);

        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, const int width, const int height)
        {
            auto* ctx     = static_cast<Window*>(glfwGetWindowUserPointer(window));
            ctx->m_width  = width;
            ctx->m_height = height;
        });

        glfwSetScrollCallback(m_window, [](GLFWwindow* window, const double x, const double y)
        {
            auto* ctx = static_cast<Window*>(glfwGetWindowUserPointer(window));
            ctx->OnScroll(x, y);
        });

        glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, const double x, const double y)
        {
            auto* ctx = static_cast<Window*>(glfwGetWindowUserPointer(window));
            ctx->OnMouseMove(x, y);
        });
    }

    void Window::Bind()
    {
        glfwMakeContextCurrent(m_window);
    }

    void Window::Draw()
    {
        glfwSwapBuffers(m_window);

        UpdateFPSCounter();

        glViewport(0, 0, m_width, m_height);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    getter_decl_(Window, Width)
    {
        return m_width;
    }

    getter_decl_(Window, Height)
    {
        return m_height;
    }

    getter_decl_(Window, Title)
    {
        return m_title.data();
    }

    setter_decl_(Window, Title)
    {
        m_title = std::string(value);
    }

    getter_decl_(Window, ShouldClose)
    {
        return glfwWindowShouldClose(m_window);
    }

    setter_decl_(Window, ShouldClose)
    {
        glfwSetWindowShouldClose(m_window, value);
    }

    bool Window::Instanced()
    {
        return s_window != nullptr;
    }

    Window& Window::Instance()
    {
        if (s_window == nullptr)
            throw std::runtime_error("Instance is null");
        return *s_window;
    }
}
