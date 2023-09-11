cmake_minimum_required(VERSION 3.26)

project (WindowsApiTest)

set(TargetName test_windows_api)

set(CMAKE_CXX_STANDARD 20)

file (GLOB_RECURSE src_files src/*.cpp)

add_executable(${TargetName} ${src_files})