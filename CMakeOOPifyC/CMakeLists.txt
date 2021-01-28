cmake_minimum_required(VERSION 3.0.0)
project(hello VERSION 0.1.0)

# set(CMAKE_CXX_STANDARD 20)
# find_package(GTest CONFIG REQUIRED)
# set(GtestLibs GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
# add_executable(Main main.cpp)
# target_link_libraries(Main ${GtestLibs})


file(READ main.c source)
message("${source}")

function(OOP)
string(REGEX REPLACE "([A-Za-z0-9_]+)\\.([A-Za-z0-9]+)\\(\\)" "\\2(&\\1)" out "${source}")
message("${out}")
file(WRITE main_oop.c "${out}")
endfunction()

OOP()
add_executable(Main main_oop.c)