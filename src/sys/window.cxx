#include <log.h>
#include <algorithm>
#include <GLFW/glfw3.h>
#include <utility>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "sys/window.h"

namespace tron::sys
{
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

    void Window::UpdateProjection()
    {
        m_projection = glm::perspective(
            glm::radians(45.0f),
            static_cast<float>(m_width) / static_cast<float>(m_height),
            0.1f,
            100.f);
    }

    Window::Window(const int w, const int h, std::string title)
        : m_window(nullptr),
          m_width(w), m_height(h),
          m_title(std::move(title)),
          m_projection(1)
    {
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

        glfwSetKeyCallback(m_window, [](GLFWwindow* window, const int key, int, int action, int)
        {
            auto* ctx = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (action == GLFW_REPEAT)
                ctx->m_repeatedKeys.push_back(key);
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
        UpdateProjection();

        glViewport(0, 0, m_width, m_height);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Window::PollEvents()
    {
        m_repeatedKeys.clear();
        glfwPollEvents();
    }

    bool Window::GetKeyDown(const int key) const
    {
        return glfwGetKey(m_window, key) == GLFW_PRESS;
    }

    bool Window::GetKey(const int key) const
    {
        return std::ranges::contains(m_repeatedKeys, key);
    }

    std::string Window::get_Title() const
    {
        return m_title;
    }

    void Window::set_Title(std::string title)
    {
        m_title = std::move(title);
    }

    glm::mat4 Window::get_Projection() const
    {
        return m_projection;
    }

    bool Window::get_ShouldClose() const
    {
        return glfwWindowShouldClose(m_window);
    }

    void Window::put_ShouldClose(const bool value) const
    {
        glfwSetWindowShouldClose(m_window, value);
    }
}
