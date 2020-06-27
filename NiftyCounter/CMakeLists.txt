cmake_minimum_required(VERSION 3.17)
project(NiftyCounter)

set(CMAKE_CXX_STANDARD 17)

add_executable(NiftyCounter main.cpp Initializer.cpp UseSingleton.cpp)