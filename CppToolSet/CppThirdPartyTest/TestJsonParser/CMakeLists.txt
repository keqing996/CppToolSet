cmake_minimum_required(VERSION 3.26)

project (TestJsonParser)

set(CMAKE_CXX_STANDARD 20)

file (GLOB_RECURSE src_files src/*.cpp)

message("output: " ${CMAKE_CURRENT_BINARY_DIR})

add_executable(test_json_parser ${src_files})

target_include_directories(test_json_parser
        PRIVATE
        ${CMAKE_SOURCE_DIR}/../../ThirdParty/src/ # library
)

add_custom_command(
        TARGET test_json_parser POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        -E copy_directory "${CMAKE_SOURCE_DIR}/config/"  "${CMAKE_CURRENT_BINARY_DIR}/config/"
)