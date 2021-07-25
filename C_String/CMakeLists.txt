cmake_minimum_required(VERSION 3.5)

project(CString)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(GTest CONFIG REQUIRED)
enable_testing()
include(GoogleTest)
# add_executable(test main.c cstring.c)
add_executable(CStringTest test.cpp cstring.c)
target_link_libraries(CStringTest PRIVATE GTest::gtest GTest::gtest_main)
gtest_discover_tests(CStringTest)