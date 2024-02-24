cmake_minimum_required(VERSION 3.25)
project(tron LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)

set(BINARY_DIR ${CMAKE_CURRENT_LIST_DIR}/bin)

IF (WIN32)
    execute_process(
            COMMAND powershell -file ${CMAKE_CURRENT_LIST_DIR}/sh/fetch.ps1
            WORKING_DIRECTORY ${CMAKE_CACHEFILE_DIR}
    )

    execute_process(
            COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_CURRENT_LIST_DIR}/dep/glm/glm
            ${CMAKE_CACHEFILE_DIR}/.tmp/inc/glm
    )

    link_directories(${CMAKE_CACHEFILE_DIR}/.tmp/lib/x64)
    include_directories(${CMAKE_CACHEFILE_DIR}/.tmp/inc)

    link_libraries(
            opengl32
            glew32.lib
            glfw3.lib)

    file(MAKE_DIRECTORY ${BINARY_DIR})

    set(DEPENDENCIES
            glew32.dll
            glfw3.dll
    )
    FOREACH (DEPENDENCY ${DEPENDENCIES})
        file(COPY        ${CMAKE_CACHEFILE_DIR}/.tmp/bin/x64/${DEPENDENCY}
             DESTINATION ${BINARY_DIR})
    ENDFOREACH ()
    unset(DEPENDENCIES)
ELSE ()
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(glfw3 REQUIRED)
    find_package(glm REQUIRED)

    link_libraries(
            glfw
            OpenGL::GL
            OpenGL::GLU
            OpenGL::OpenGL
            glm::glm
            GLEW::GLEW)
ENDIF ()

file(GLOB_RECURSE SOURCES src/*.cxx)

add_executable(tron ${SOURCES})
target_include_directories(tron PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/inc)
set_target_properties(tron PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${BINARY_DIR}
        ARCHIVE_OUTPUT_DIRECTORY ${BINARY_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR})

add_custom_command(
        TARGET tron
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_LIST_DIR}/res
            ${BINARY_DIR}/res
)
