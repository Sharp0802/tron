#include <iostream>

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "log.h"
#include "shader.h"
#include "program.h"
#include "def.h"
#include "vertexbuffer.h"
#include "vertexarray.h"

#if _WIN32
#include <windows.h>
#endif

#define TITLE "трон"

#define RED "\x1b[31m"
#define CLR "\x1b[0m"

int g_glWidth = 640;
int g_glHeight = 480;

using namespace tron;

void UpdateFPSCounter(GLFWwindow* window)
{
	static double prev = glfwGetTime();
	static int    cFPS;
	static char   buf[BUFSIZ];

	double current = glfwGetTime();
	double elapsed = current - prev;

	if (elapsed > 0.25)
	{
		prev = current;
		double fps = (double)cFPS / elapsed;

		sprintf(buf, TITLE " @ fps: %.2lf", fps);
		glfwSetWindowTitle(window, buf);
		cFPS = 0;
	}

	cFPS++;
}

int main()
{
#if _WIN32
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE)
		throw std::system_error(static_cast<int>(GetLastError()), std::system_category());
	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode))
		throw std::system_error(static_cast<int>(GetLastError()), std::system_category());
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode))
		throw std::system_error(static_cast<int>(GetLastError()), std::system_category());
#endif

	glfwSetErrorCallback([](int err, const char* desc) {
		log::fw.err() << std::hex << err << std::dec << ": " << desc << std::endl;
	});

    if (!glfwInit())
    {
        log::fw.err() << "glfwInit(): Couldn't start glfw3" << std::endl;
        return 1;
    }

	/*
	 * To support MacOS, We should use OpenGL 3.2 core profile.
	 * This let MacOS to use OpenGL 4.
	 */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(g_glWidth, g_glHeight, TITLE, nullptr, nullptr);
    if (!window)
    {
        log::fw.err() << "glfwCreateWindow(): Couldn't create window" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

	log::LogGLParameters();

	glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		g_glWidth = width;
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


    float points[] = {
         0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f
    };

	VertexBuffer vbo(VertexBufferTarget::ARRAY_BUFFER);
	vbo.Buffer(points, sizeof points);
	vbo.Bind();

	VertexArray vao = {
		VertexAttributeInfo::Create<glm::vec3>(),
		VertexAttributeInfo::Create<glm::vec3>(),
	};
	vao.Bind();

	Program program;

	auto& frag = program.AttachShader("sample.f.glsl");
	auto& vert = program.AttachShader("sample.v.glsl");

	program.Link();
	program.Use();
	program.Validate();


    while (!glfwWindowShouldClose(window))
    {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, 1);

		UpdateFPSCounter(window);
		glViewport(0, 0, g_glWidth, g_glHeight);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		program.Use();

		vao.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
		glfwPollEvents();
	}

    glfwTerminate();
    return 0;
}
