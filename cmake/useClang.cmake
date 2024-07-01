find_program(CLANG_C_PATH clang)
set(CMAKE_C_COMPILER ${CLANG_C_PATH})

find_program(CLANG_CXX_PATH clang++)
set(CMAKE_CXX_COMPILER ${CLANG_CXX_PATH})