cmake_minimum_required(VERSION 3.26)

project(HexoMarkDownTool)

set(TargetName tool_hexo_md)

set(CMAKE_CXX_STANDARD 20)

file (GLOB_RECURSE src_files src/*.cpp)
file (GLOB_RECURSE util_src_files ../CppUtils/Util/*.cpp)

add_executable(${TargetName} ${src_files} ${util_src_files})

target_include_directories(${TargetName}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../CppUtils/Util/ # util
)

# release copy artifacts
if (CMAKE_BUILD_TYPE STREQUAL Release)
    add_custom_command(
            TARGET ${TargetName} POST_BUILD
            COMMAND ${CMAKE_COMMAND}
            -E copy
            $<TARGET_FILE:${TargetName}>
            "${CMAKE_CURRENT_SOURCE_DIR}/../Artifacts/${TargetName}.exe"
    )
endif ()