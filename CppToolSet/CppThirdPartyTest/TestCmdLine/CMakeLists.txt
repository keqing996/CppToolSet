cmake_minimum_required(VERSION 3.26)

project (TestCmdLine)

set(CMAKE_CXX_STANDARD 20)

file (GLOB_RECURSE src_files src/*.cpp)

add_executable(test_cmd_line ${src_files})

target_include_directories(test_cmd_line
        PRIVATE
        ${CMAKE_SOURCE_DIR}/../../ThirdParty/src/ # library
)