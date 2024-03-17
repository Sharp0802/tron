cmake_minimum_required(VERSION 3.25)
project(tron LANGUAGES CXX)


set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)


set(CMAKE_CXX_STANDARD 26)

set(BINARY_DIR ${CMAKE_CURRENT_LIST_DIR}/bin/${CMAKE_BUILD_TYPE})

###################################################
# Build Profiles                                  #
###################################################
IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-D_DEBUG)
    add_compile_options(-g -ggdb -Og)
ELSE()
    add_definitions(-D_RELEASE)
ENDIF()
add_compile_options(-Wno-gnu-string-literal-operator-template -fdeclspec -fms-extensions)

###################################################
# Platform Dependants                             #
###################################################
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

file(
        DOWNLOAD https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
        ${CMAKE_CACHEFILE_DIR}/.tmp/inc/stb_image.h
)
file(
        DOWNLOAD https://raw.githubusercontent.com/dotnet/runtime/main/src/native/corehost/nethost/nethost.h
        ${CMAKE_CACHEFILE_DIR}/.tmp/inc/nethost.h
)
file(
        DOWNLOAD https://raw.githubusercontent.com/dotnet/runtime/main/src/native/corehost/coreclr_delegates.h
        ${CMAKE_CACHEFILE_DIR}/.tmp/inc/coreclr_delegates.h
)
file(
        DOWNLOAD https://raw.githubusercontent.com/dotnet/runtime/main/src/native/corehost/hostfxr.h
        ${CMAKE_CACHEFILE_DIR}/.tmp/inc/hostfxr.h
)
include_directories(${CMAKE_CACHEFILE_DIR}/.tmp/inc)

###################################################
# Generic Configurations                          #
###################################################
file(GLOB_RECURSE SOURCES   src/*.cxx)
file(GLOB_RECURSE RESOURCES res/**)

add_executable(tron ${SOURCES})
target_precompile_headers(tron PRIVATE inc/pch.h)
target_include_directories(tron PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/inc)
set_target_properties(tron PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${BINARY_DIR}
        ARCHIVE_OUTPUT_DIRECTORY ${BINARY_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR})

add_custom_command(
        TARGET tron
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
            ${CMAKE_CURRENT_LIST_DIR}/res
            ${BINARY_DIR}/res
        DEPENDS ${RESOURCES}
)
