cmake_minimum_required(VERSION 3.27)
project(tron)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(useClang)

add_subdirectory(sub/glm)
add_subdirectory(cmake/stb)
add_subdirectory(tron)