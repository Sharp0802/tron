cmake_minimum_required(VERSION 3.25)
project(tron-cg LANGUAGES CXX)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 26)

add_executable(tron-cg cg/cg.cxx)
target_link_libraries(tron-cg PRIVATE clang)
target_compile_options(tron-cg PRIVATE -O3 -march=native)
