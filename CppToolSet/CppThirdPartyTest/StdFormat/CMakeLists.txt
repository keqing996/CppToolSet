cmake_minimum_required(VERSION 3.26)

project (StdFormat)

set(CMAKE_CXX_STANDARD 20)

file (GLOB_RECURSE src_files src/*.cpp)

add_executable(test_std_format ${src_files})