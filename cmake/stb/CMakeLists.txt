project(stb)

set(STB_PATH "${CMAKE_SOURCE_DIR}/sub/stb")

file(GLOB HEADERS "${STB_PATH}/*.h")

add_library(stb INTERFACE ${HEADERS})
target_include_directories(stb INTERFACE "${STB_PATH}")