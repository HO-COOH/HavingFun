cmake_minimum_required(VERSION 3.10)

project(ReinventSTL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Main main.cpp "Utility.hpp" "TMP.hpp" "UninitializedMemory.hpp" "InitializedMemory.hpp" "Algorithm.hpp")