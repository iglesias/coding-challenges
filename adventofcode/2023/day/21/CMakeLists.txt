cmake_minimum_required(VERSION 3.15..3.27)
project(aoc-2023-21 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
#set(CMAKE_CXX_FLAGS "-Wall -Wconversion -Wextra -pedantic -fsanitize=address,pointer-overflow,signed-integer-overflow,undefined")
find_package(pybind11 REQUIRED)
add_executable(step_counter step_counter.cpp)
target_link_libraries(step_counter PRIVATE pybind11::embed)
set(CMAKE_BUILD_TYPE Release)
