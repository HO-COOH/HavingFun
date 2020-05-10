cmake_minimum_required(VERSION 3.5)

project(CString)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_definitions(-DDEBUG)
add_executable(test main.c cstring.c)
