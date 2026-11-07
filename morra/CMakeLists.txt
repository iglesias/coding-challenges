cmake_minimum_required(VERSION 3.14..3.31)

project(morra LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

enable_testing()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(Coverage)

add_subdirectory(src)
add_executable(morra morra.cpp)
target_link_libraries(morra morra_lib)

add_subdirectory(test)
