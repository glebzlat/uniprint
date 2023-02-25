cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(uniprint LANGUAGES CXX)

add_library(uniprint INTERFACE)
target_include_directories(uniprint INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(uniprint INTERFACE
    -Wall -Wpedantic -Wextra)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)
