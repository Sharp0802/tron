#include <oop/actor.h>
#include <oop/components/camera.h>
#include <oop/components/meshrenderer.h>
#include <oop/components/transform.h>
#include <oop/specials/material.h>
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

constexpr std::initializer_list<uint8_t> indices = {
    RECT(0), RECT(1), RECT(2), RECT(3), RECT(4), RECT(5),
};

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

    Actor actor({
        .Enabled = [](CObject* self)
        {
            auto* actor = dynamic_cast<Actor*>(self);
            actor->AddComponent(new components::Transform(actor, nullptr));

            auto* tex = new specials::Texture2D("res/tex/brick_wall.jpg");
            auto* material = new specials::Material;
            material->Program->AttachShader("res/sha/sample.f.glsl");
            material->Program->AttachShader("res/sha/sample.v.glsl");
            material->Program->Link();
            material->Program->Use();
            material->Program->Validate();
            material->Texture = tex;

            auto* mesh = new specials::Mesh({
                specials::VertexAttributeInfo::Create<glm::vec3>(),
                specials::VertexAttributeInfo::Create<glm::vec3>(),
                specials::VertexAttributeInfo::Create<glm::vec2>()
            });
            mesh->Bind();
            mesh->VBO->Buffer(points, sizeof points);
            mesh->EBO->Buffer(indices);

            auto* renderer = new components::MeshRenderer(actor);
            renderer->Material = material;
            renderer->Mesh     = mesh;
            actor->AddComponent(renderer);
        }
    });

    Actor cameraActor({
        .Enabled = [](CObject* self)
        {
            auto* actor = dynamic_cast<Actor*>(self);
            actor->AddComponent(new components::Transform(actor, nullptr));

            auto* camera = new components::Camera(actor);
            actor->AddComponent(camera);
            camera->Bind();

            auto* transform = dynamic_cast<components::Transform*>(actor->
                GetComponent(GetType<components::Transform>()));
            transform->Position = glm::vec3(0.0f, 0.0f, -3.0f);

            sys::Window::Instance().SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        },
        .Updated = [](CObject* self, float delta)
        {
            static glm::vec2 lastMouse;

            auto* actor = dynamic_cast<Actor*>(self);

            if (sys::Input::GetKey(GLFW_KEY_ESCAPE))
                sys::Window::Instance().ShouldClose = true;

            auto* transform = dynamic_cast<components::Transform*>(actor->
                GetComponent(GetType<components::Transform>()));

            if (sys::Input::GetKey(GLFW_KEY_W))
                transform->LocalPosition += glm::vec3(0, 0, delta);
            if (sys::Input::GetKey(GLFW_KEY_S))
                transform->LocalPosition += glm::vec3(0, 0, -delta);
            if (sys::Input::GetKey(GLFW_KEY_A))
                transform->LocalPosition += glm::vec3(delta, 0, 0);
            if (sys::Input::GetKey(GLFW_KEY_D))
                transform->LocalPosition += glm::vec3(-delta, 0, 0);

            if (sys::Input::GetKey(GLFW_KEY_SPACE))
                transform->Position += glm::vec3(0, delta, 0);
            if (sys::Input::GetKey(GLFW_KEY_LEFT_SHIFT))
                transform->Position += glm::vec3(0, -delta, 0);

            auto* camera = dynamic_cast<components::Camera*>(actor->
                GetComponent(GetType<components::Camera>()));

            camera->FoV += sys::Input::GetScroll();

            auto rot = transform->Rotation;
            auto off = (lastMouse - sys::Input::GetMouseMove()) * .1f * delta;
            rot.x -= off.y;
            rot.y -= -off.x;
            transform->Rotation = rot;
            lastMouse = sys::Input::GetMouseMove();
        }
    });

    double last = glfwGetTime();
    while (!window.ShouldClose)
    {
        const double time  = glfwGetTime();
        const double delta = time - last;
        const auto deltaF  = static_cast<float>(delta);
        last               = time;

        LimitFrame(60, delta);

        sys::Input::Poll();

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
