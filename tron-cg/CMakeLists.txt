cmake_minimum_required(VERSION 3.25)
project(tron-cg)

add_executable(tron-cg main.cxx)
target_compile_options(tron-cg PRIVATE -O3 -march=native -fopenmp=libomp)
target_link_libraries(tron-cg PRIVATE clang omp)
