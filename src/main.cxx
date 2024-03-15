#include <iostream>

#define GLFW_DLL

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

#include <thread>

#if _WIN32
#include <windows.h>
#endif

#define TITLE "трон"

GLFWwindow* g_window = nullptr;

int g_glWidth  = 640;
int g_glHeight = 480;

glm::mat4 g_projection;

using namespace tron;

void UpdateFPSCounter(GLFWwindow* window)
{
	static double prev = glfwGetTime();
	static int cFPS;
	static char buf[BUFSIZ];

	double current = glfwGetTime();
	double elapsed = current - prev;

	if (elapsed > 0.25)
	{
		prev       = current;
		double fps = (double)cFPS / elapsed;

		sprintf(buf, TITLE " @ fps: %.2lf", fps);
		glfwSetWindowTitle(window, buf);
		cFPS = 0;
	}

	cFPS++;
}

void UpdateProjection()
{
	g_projection = glm::perspective(
			glm::radians(45.0f),
			static_cast<float>(g_glWidth) / static_cast<float>(g_glHeight),
			0.1f,
			100.f);
}

void BeginDraw()
{
	UpdateFPSCounter(g_window);
	UpdateProjection();

	glViewport(0, 0, g_glWidth, g_glHeight);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void EndDraw()
{
	glfwSwapBuffers(g_window);
}

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

	if (!glfwInit())
	{
		log::fw.err() << "glfwInit(): Couldn't start glfw3" << std::endl;
		return 1;
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

	g_window = glfwCreateWindow(g_glWidth, g_glHeight, TITLE, nullptr, nullptr);
	if (!g_window)
	{
		log::fw.err() << "glfwCreateWindow(): Couldn't create window" << std::endl;
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(g_window);

	log::LogGLParameters();

	glfwSetWindowSizeCallback(g_window, [](GLFWwindow* window, int width, int height) {
		g_glWidth  = width;
		g_glHeight = height;
	});

	glewExperimental = GL_TRUE;
	glewInit();

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version  = glGetString(GL_VERSION);

	log::gl.out() << "Renderer: " << renderer << std::endl;
	log::gl.out() << "Version : " << version << std::endl;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

#define U 0.3f

	float points[] = {
			-U, U, U, 0, 1, 0, 1, 1,
			U, U, U, 0, 0, 1, 1, 0,
			U, -U, U, 1, 0, 0, 0, 0,
			-U, -U, U, 1, 0, 0, 0, 1,

			U, U, U, 0, 1, 0, 1, 1,
			U, U, -U, 0, 0, 1, 1, 0,
			U, -U, -U, 1, 0, 0, 0, 0,
			U, -U, U, 1, 0, 0, 0, 1,

			U, U, -U, 0, 1, 0, 1, 1,
			-U, U, -U, 0, 0, 1, 1, 0,
			-U, -U, -U, 1, 0, 0, 0, 0,
			U, -U, -U, 1, 0, 0, 0, 1,

			-U, U, -U, 0, 1, 0, 1, 1,
			-U, U, U, 0, 0, 1, 1, 0,
			-U, -U, U, 1, 0, 0, 0, 0,
			-U, -U, -U, 1, 0, 0, 0, 1,

			-U, U, -U, 0, 1, 0, 1, 1,
			U, U, -U, 0, 0, 1, 1, 0,
			U, U, U, 1, 0, 0, 0, 0,
			-U, U, U, 1, 0, 0, 0, 1,

			-U, -U, U, 0, 1, 0, 1, 1,
			U, -U, U, 0, 0, 1, 1, 0,
			U, -U, -U, 1, 0, 0, 0, 0,
			-U, -U, -U, 1, 0, 0, 0, 1,
	};

#define R_P(p, i) (p+(i*4))
#define R(i) R_P(0,i),R_P(1,i),R_P(2,i),R_P(0,i),R_P(2,i),R_P(3,i)

	std::vector<uint16_t> indices = {
			R(0), R(1), R(2), R(3), R(4), R(5),
	};

	auto mesh = std::make_shared_mesh({
			VertexAttributeInfo::Create<glm::vec3>(),
			VertexAttributeInfo::Create<glm::vec3>(),
			VertexAttributeInfo::Create<glm::vec2>(),
	});
	mesh->Bind();
	mesh->VBO.Buffer(points, sizeof points);
	mesh->EBO.Buffer(indices);


	auto program = std::make_shared<Program>();

	program->AttachShader("sample.f.glsl");
	auto& vert = program->AttachShader("sample.v.glsl");

	program->Link();
	program->Use();
	program->Validate();

	auto tex      = std::make_shared<Texture2D>("res/tex/brick_wall.jpg");
	auto material = std::make_shared<Material>(program, tex);


	glm::mat4 view(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));


	auto object = std::make_shared<MeshRenderer>(material, mesh);
	

	object->Transform->Position = glm::vec3(1, 0, 0);
	object->Transform->Rotation = glm::vec3(glm::radians(45.f), 0, 0);

	double last = glfwGetTime();

	while (!glfwWindowShouldClose(g_window))
	{
		const double time = glfwGetTime();
		const double delta = time - last;
		last = time;

		LimitFrame(60, delta);
		
		// OnUpdate
		{
			glfwPollEvents();
			if (glfwGetKey(g_window, GLFW_KEY_ESCAPE))
				glfwSetWindowShouldClose(g_window, 1);

			object->Transform->Rotation += glm::vec3(0, delta, 0);
		}

		BeginDraw();
		// OnDraw
		{
			object->Bind(g_projection, view);
			object->Draw();
		}
		EndDraw();

	}

	glfwTerminate();
	return 0;
}