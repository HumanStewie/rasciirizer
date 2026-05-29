cmake_minimum_required(VERSION 3.24)
project(rasciirizer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(GCC_COMPILE_FLAGS -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion -pedantic-errors)

add_executable(rasciirizer src/main.cpp)
target_include_directories(rasciirizer PRIVATE include/ PUBLIC examples/)
target_compile_options(rasciirizer PRIVATE ${GCC_COMPILE_FLAGS})
