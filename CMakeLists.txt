cmake_minimum_required(VERSION 3.24)
project(rasciirizer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(GCC_COMPILE_FLAGS -Wall)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

add_executable(rasciirizer src/main.cpp "src/AssetImporter.cpp" "include/AssetImporter.h")
target_include_directories(rasciirizer PRIVATE include/ examples/ vendored/)
target_compile_options(rasciirizer PRIVATE ${GCC_COMPILE_FLAGS})
target_link_libraries(rasciirizer PRIVATE SDL3::SDL3)
