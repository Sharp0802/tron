cmake_minimum_required(VERSION 3.25)
project(tron LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)

find_package(glfw3  REQUIRED)
find_package(GLEW   REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm    REQUIRED)

set(BINARY_DIR ${CMAKE_CURRENT_LIST_DIR}/bin)

file(MAKE_DIRECTORY ${BINARY_DIR})
file(COPY           ${CMAKE_CURRENT_LIST_DIR}/res
     DESTINATION    ${BINARY_DIR})

add_executable(tron
    src/main.cxx
    src/resource.cxx
    inc/resource.h
    src/shader.cc
    inc/shader.h
    inc/GL/except.h
    inc/GL/stdexcept.h
    src/memorymappedfile.cc
    inc/memorymappedfile.h)
target_include_directories(tron PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/inc)
set_target_properties(tron PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${BINARY_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR})
target_link_libraries(tron PRIVATE
    glfw
    OpenGL::GL
    OpenGL::GLU
    OpenGL::OpenGL
    glm::glm
    GLEW::GLEW)
