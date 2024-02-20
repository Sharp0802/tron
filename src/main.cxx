#include <iostream>

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include "shader.h"

#define TITLE "трон"

#define RED "\x1b[31m"
#define CLR "\x1b[0m"

int main()
{
    if (!glfwInit())
    {
        std::cerr << RED "glfwInit(): Couldn't start glfw3" CLR << std::endl;
        return 1;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, TITLE, nullptr, nullptr);
    if (!window)
    {
        std::cerr << RED "glfwCreateWindow(): Couldn't create window" CLR << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version  = glGetString(GL_VERSION);

    std::cout << "Renderer: " << renderer << std::endl
              << "Version : " << version  << std::endl;

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

    const tron::Shader vs("sample.v", GL_VERTEX_SHADER);
    const tron::Shader fs("sample.f", GL_FRAGMENT_SHADER);

    GLuint programme = glCreateProgram();
    glAttachShader(programme, fs.GetHandle());
    glAttachShader(programme, vs.GetHandle());
    glLinkProgram(programme);
    glUseProgram(programme);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwPollEvents();

        glfwSwapBuffers(window);
    }

    glDeleteProgram(programme);
    glfwTerminate();
    return 0;
}
