cmake_minimum_required(VERSION 3.25)
project(TronEngine)

set(BINARY_DIR ${CMAKE_BINARY_DIR}/bin/)

###################################################
# Compilation Profile                             #
###################################################
set(CMAKE_CXX_STANDARD 26)

###################################################
# Compilation Definition/Options                  #
###################################################
IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-D_DEBUG)
    add_compile_options(-g -ggdb -Og)
ELSE()
    add_definitions(-D_RELEASE)
ENDIF()
add_compile_options(-Wno-gnu-string-literal-operator-template -fdeclspec -fms-extensions)

###################################################
# Subprojects                                     #
###################################################
add_subdirectory(tron)
add_subdirectory(tron-sample)
