#include <oop/actor.h>
#include <oop/components/camera.h>
#include <oop/components/meshrenderer.h>
#include <oop/components/transform.h>
#include <oop/specials/material.h>
#include <oop/specials/program.h>
#include <oop/specials/texture2d.h>
#include <oop/sys/input.h>
#include <oop/sys/window.h>

#include "pch.h"

#include "log.h"

#define TITLE "трон"

using namespace tron;
using namespace tron::oop;

void LimitFrame(const int frame, const double delta)
{
    if (const auto delay = 1.0 / frame - delta; delay > 0)
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int64_t>(delay * 1000)));
}

void InitializeTerminal()
{
#if _WIN32
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE) return;
	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode)) return;
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode)) return;
#endif
}

int main()
{
    InitializeTerminal();

    glfwSetErrorCallback([](int err, const char* desc)
    {
        log::fw.err() << std::hex << err << std::dec << ": " << desc << std::endl;
    });

    sys::Window window;
    window.Title = TITLE;
    window.Bind();

    log::LogGLParameters();

    // Initialize GL
    {
        glewExperimental = GL_TRUE;
        glewInit();

        const GLubyte* renderer = glGetString(GL_RENDERER);
        const GLubyte* version  = glGetString(GL_VERSION);

        log::gl.out() << "Renderer: " << renderer << std::endl;
        log::gl.out() << "Version : " << version << std::endl;

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }

    specials::Material material;
    material.Program->AttachShader("res/sha/sample.f.glsl");
    material.Program->AttachShader("res/sha/sample.v.glsl");
    material.Program->Link();
    material.Program->Use();
    material.Program->Validate();
    specials::Texture2D tex("res/tex/brick_wall.jpg");
    material.Texture = &tex;

#define UNIT 1.0f

    float points[] = {
        -UNIT, UNIT, UNIT, 0, 1, 0, 1, 1,
        UNIT, UNIT, UNIT, 0, 0, 1, 1, 0,
        UNIT, -UNIT, UNIT, 1, 0, 0, 0, 0,
        -UNIT, -UNIT, UNIT, 1, 0, 0, 0, 1,

        UNIT, UNIT, UNIT, 0, 1, 0, 1, 1,
        UNIT, UNIT, -UNIT, 0, 0, 1, 1, 0,
        UNIT, -UNIT, -UNIT, 1, 0, 0, 0, 0,
        UNIT, -UNIT, UNIT, 1, 0, 0, 0, 1,

        UNIT, UNIT, -UNIT, 0, 1, 0, 1, 1,
        -UNIT, UNIT, -UNIT, 0, 0, 1, 1, 0,
        -UNIT, -UNIT, -UNIT, 1, 0, 0, 0, 0,
        UNIT, -UNIT, -UNIT, 1, 0, 0, 0, 1,

        -UNIT, UNIT, -UNIT, 0, 1, 0, 1, 1,
        -UNIT, UNIT, UNIT, 0, 0, 1, 1, 0,
        -UNIT, -UNIT, UNIT, 1, 0, 0, 0, 0,
        -UNIT, -UNIT, -UNIT, 1, 0, 0, 0, 1,

        -UNIT, UNIT, -UNIT, 0, 1, 0, 1, 1,
        UNIT, UNIT, -UNIT, 0, 0, 1, 1, 0,
        UNIT, UNIT, UNIT, 1, 0, 0, 0, 0,
        -UNIT, UNIT, UNIT, 1, 0, 0, 0, 1,

        -UNIT, -UNIT, UNIT, 0, 1, 0, 1, 1,
        UNIT, -UNIT, UNIT, 0, 0, 1, 1, 0,
        UNIT, -UNIT, -UNIT, 1, 0, 0, 0, 0,
        -UNIT, -UNIT, -UNIT, 1, 0, 0, 0, 1,
    };

#define RECT_P(p, i) (p+(i*4))
#define RECT(i) RECT_P(0,i),RECT_P(1,i),RECT_P(2,i),RECT_P(0,i),RECT_P(2,i),RECT_P(3,i)

    std::initializer_list<uint16_t> indices = {
        RECT(0), RECT(1), RECT(2), RECT(3), RECT(4), RECT(5),
    };

    specials::Mesh mesh({
        specials::VertexAttributeInfo::Create<glm::vec3>(),
        specials::VertexAttributeInfo::Create<glm::vec3>(),
        specials::VertexAttributeInfo::Create<glm::vec2>()
    });
    mesh.Bind();
    mesh.VBO->Buffer(points, sizeof points);
    mesh.EBO->Buffer(indices);

    Actor actor({});

    components::MeshRenderer renderer(&actor);
    renderer.Material = &material;
    renderer.Mesh = &mesh;
    actor.TryAddComponent(&renderer);

    Actor cameraActor({});
    components::Camera camera(&cameraActor);
    camera.Bind();

    auto* cameraTransform = dynamic_cast<components::Transform*>(cameraActor.GetComponent(
        GetType<components::Transform>()));
    cameraTransform->Position = glm::vec3(0.0f, 0.0f, -3.0f);

    double last = glfwGetTime();
    while (!window.ShouldClose)
    {
        const double time  = glfwGetTime();
        const double delta = time - last;
        const auto deltaF  = static_cast<float>(delta);
        last               = time;

        LimitFrame(60, delta);

        // OnUpdate
        {
            sys::Input::Poll();
            if (sys::Input::GetKey(GLFW_KEY_ESCAPE))
                window.ShouldClose = true;

            if (sys::Input::GetKey(GLFW_KEY_W))
                cameraTransform->LocalPosition += glm::vec3(0, 0, delta);
            if (sys::Input::GetKey(GLFW_KEY_S))
                cameraTransform->LocalPosition += glm::vec3(0, 0, -delta);
            if (sys::Input::GetKey(GLFW_KEY_A))
                cameraTransform->LocalPosition += glm::vec3(delta, 0, 0);
            if (sys::Input::GetKey(GLFW_KEY_D))
                cameraTransform->LocalPosition += glm::vec3(-delta, 0, 0);

            if (sys::Input::GetKey(GLFW_KEY_SPACE))
                cameraTransform->Position += glm::vec3(0, delta, 0);
            if (sys::Input::GetKey(GLFW_KEY_LEFT_SHIFT))
                cameraTransform->Position += glm::vec3(0, -delta, 0);
        }

        // OnDraw
        window.Draw();
        {
            actor.Update(deltaF);
            cameraActor.Update(deltaF);
        }
    }

    glfwTerminate();
    return 0;
}
