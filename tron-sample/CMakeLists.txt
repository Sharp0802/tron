cmake_minimum_required(VERSION 3.25)
project(tron-sample)

add_executable(tron-sample main.cxx)
target_include_directories(tron-sample PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../tron/inc/)
target_link_libraries(tron-sample PRIVATE tron)
set_target_properties(tron-sample PROPERTIES
    OUTPUT_NAME "tron"
    LIBRARY_OUTPUT_DIRECTORY ${BINARY_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR}
)
