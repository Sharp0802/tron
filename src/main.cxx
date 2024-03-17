#include <iostream>

#include "pch.h"

#include "log.h"
#include "mesh.h"
#include "program.h"
#include "texture2d.h"
#include "vertexarray.h"
#include "vertexbuffer.h"

#include "def.h"
#include "material.h"
#include "meshrenderer.h"
#include "transform.h"
#include "sys/window.h"

#if _WIN32
#include <windows.h>
#endif

#define TITLE "трон"

using namespace tron;

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

	glfwSetErrorCallback([](int err, const char* desc) {
		log::fw.err() << std::hex << err << std::dec << ": " << desc << std::endl;
	});


	sys::Window window(640, 480, TITLE);
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

	ptr<MeshRenderer> object;
	{
		auto program = std::make_shared<Program>();
		program->AttachShader("sample.f.glsl");
		program->AttachShader("sample.v.glsl");
		program->Link();
		program->Use();
		program->Validate();

		auto tex      = std::make_shared<Texture2D>("res/tex/brick_wall.jpg");
		auto material = std::make_shared<Material>(program, tex);

		object = std::make_shared<MeshRenderer>(material, Mesh::GetPrimitiveObject(Primitive::Cube));
		object->Transform->Scale = glm::vec3(0.3f);
		object->Transform->Rotation = glm::vec3(glm::radians(45.f), 0, 0);
	}

	glm::mat4 view(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	double last = glfwGetTime();
	while (!window.ShouldClose)
	{
		const double time = glfwGetTime();
		const double delta = time - last;
		last = time;

		LimitFrame(60, delta);
		
		// OnUpdate
		{
			window.PollEvents();
			if (window.GetKeyDown(GLFW_KEY_ESCAPE))
				window.ShouldClose = true;

			object->Transform->Rotation += glm::vec3(0, delta, 0);

			auto pos = object->Transform->Position;
			pos.y = sinf(glfwGetTime()) * 0.3f;
			object->Transform->Position = pos;
		}

		// OnDraw
		window.Draw();
		{
			object->Bind(window.Projection, view);
			object->Draw();
		}
	}

	glfwTerminate();
	return 0;
}