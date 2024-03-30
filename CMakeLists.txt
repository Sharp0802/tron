cmake_minimum_required(VERSION 3.25)
project(TronEngine)

add_compile_options(-Wno-gnu-string-literal-operator-template -fdeclspec -fms-extensions)

include(tron/CMakeLists.txt)
