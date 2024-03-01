#include <iostream>

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "log.h"
#include "program.h"
#include "vertexbuffer.h"
#include "vertexarray.h"
#include "mesh.h"

#if _WIN32
#include <windows.h>
#endif

#define TITLE "трон"

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
	 * To support macOS, We should use OpenGL 3.2 core profile.
	 * This let macOS use OpenGL 4.
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

	log::gl.out() << "IRenderable: " << renderer << std::endl;
	log::gl.out() << "Version : " << version << std::endl;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);


    float points[] = {
			0.5f,  0.5f,  0.0f, 0, 1, 0, 1, 1,
			0.5f,  -0.5f, 0.0f, 0, 0, 1, 1, 0,
			-0.5f, -0.5f, 0.0f, 1, 0, 0, 0, 0,
			-0.5f, 0.5f,  0.0f, 1, 0, 0, 0, 1,
    };

	std::vector<uint16_t> indices = {
		0, 1, 2,
		0, 2, 3
	};

	Mesh mesh = {
			VertexAttributeInfo::Create<glm::vec3>(),
			VertexAttributeInfo::Create<glm::vec3>(),
			VertexAttributeInfo::Create<glm::vec2>(),
	};
	mesh.Bind();
	mesh.VBO().Buffer(points, sizeof points);
	mesh.EBO().Buffer(indices);


	int width, height, nrChannels;
	auto* data = stbi_load("res/tex/brick_wall.jpg", &width, &height, &nrChannels, 0);
	if (!data)
	{
		log::stbi.err() << "Failed to load texture" << std::endl;
	}

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	Program program;

	program.AttachShader("sample.f.glsl");
	program.AttachShader("sample.v.glsl");

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

		glBindTexture(GL_TEXTURE_2D, tex);
		mesh.Draw();

        glfwSwapBuffers(window);
		glfwPollEvents();
	}

    glfwTerminate();
    return 0;
}
