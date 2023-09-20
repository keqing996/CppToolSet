cmake_minimum_required(VERSION 3.26)

project(WindowsApiLib)

set(CMAKE_CXX_STANDARD 20)

set(TargetName winapi)

file (GLOB_RECURSE src_files src/*.cpp)

add_library(${TargetName} STATIC ${src_files})

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    set(FinalLibName ${TargetName}_d.lib)
else ()
    set(FinalLibName ${TargetName}.lib)
endif ()

add_custom_command(
        TARGET ${TargetName} POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        -E copy
        $<TARGET_FILE:${TargetName}>
        "${CMAKE_CURRENT_SOURCE_DIR}/../../Artifacts/Library/WindowsApi/lib/${FinalLibName}"
)

add_custom_command(
        TARGET ${TargetName} POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/include/"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../Artifacts/Library/WindowsApi/include/"
)