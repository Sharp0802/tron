#include <iostream>

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include "log.h"
#include "shader.h"
#include "program.h"

#define TITLE "трон"

#define RED "\x1b[31m"
#define CLR "\x1b[0m"

int g_glWidth = 640;
int g_glHeight = 480;

#if _WIN32
#include <windows.h>
#endif

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
		return GetLastError();
	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode))
		return GetLastError();
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode))
		return GetLastError();
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
	 * To support MacOS, We should use OpenGL 3.5 core profile.
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
         0.0f,  0.5f,  0.0f,
         0.5f, -0.5f,  0.0f,
        -0.5f, -0.5f,  0.0f
    };

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof points, points, GL_STATIC_DRAW);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	Program program;
	program.AttachResourceRoot("res");
	program.Use();

    while (!glfwWindowShouldClose(window))
    {
		UpdateFPSCounter(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, g_glWidth, g_glHeight);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwPollEvents();

        glfwSwapBuffers(window);

		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, 1);
	}

    glfwTerminate();
    return 0;
}
