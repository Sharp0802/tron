project(tron)

find_package(OpenGL REQUIRED)
find_package(GLEW   REQUIRED)
find_package(glfw3  REQUIRED)

file(GLOB SOURCES src/*.cxx)
file(GLOB HEADERS *.h)

add_executable(tron ${SOURCES} ${HEADERS})
set_target_properties(tron PROPERTIES
    CXX_STANDARD 26
)
target_include_directories(tron PRIVATE .)
target_precompile_headers(tron PRIVATE pch.h)
target_link_libraries(tron PRIVATE
        # SUBMODULES
        glm::glm
        stb
        # PACKAGES
        OpenGL::GL
        OpenGL::GLU
        OpenGL::OpenGL
        GLEW::GLEW
        glfw
)
