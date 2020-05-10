cmake_minimum_required(VERSION 3.15)

project(Timer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Threads REQUIRED)
add_executable(test test.cpp)
target_link_libraries(test Threads::Threads)
message(${CMAKE_CXX_COMPILE_FEATURES})