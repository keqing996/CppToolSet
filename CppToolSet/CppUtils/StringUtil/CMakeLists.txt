cmake_minimum_required(VERSION 3.26)

project(StringUtil)

set(TargetName string_util)

set(CMAKE_CXX_STANDARD 20)

file (GLOB_RECURSE src_files src/*.cpp)

add_executable(${TargetName} ${src_files} ${res_files})

# copy hpp
add_custom_command(
        TARGET ${TargetName} POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/include/"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../Artifacts/StringUtil/"
)