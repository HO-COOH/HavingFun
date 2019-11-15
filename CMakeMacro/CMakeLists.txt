cmake_minimum_required(VERSION 3.0.0)
project(Macro VERSION 0.1.0)

add_executable(Macro main.cpp)

add_compile_definitions(MODE=${CMAKE_BUILD_TYPE})