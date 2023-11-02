cmake_minimum_required(VERSION 3.26)

project(VsNewProjWin32Deleter)

set(TargetName tool_vs_win32_deleter)
set(CMAKE_CXX_STANDARD 20)

file (GLOB_RECURSE src_files src/*.cpp)

add_executable(${TargetName} ${src_files})

target_include_directories(${TargetName}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/src/
)

# copy config
add_custom_command(
        TARGET ${TargetName} POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/testxml/"  "${CMAKE_CURRENT_BINARY_DIR}/testxml/"
)

# release copy artifacts
if (CMAKE_BUILD_TYPE STREQUAL Release)
    add_custom_command(
            TARGET ${TargetName} POST_BUILD
            COMMAND ${CMAKE_COMMAND}
            -E copy
            $<TARGET_FILE:${TargetName}>
            "${CMAKE_CURRENT_SOURCE_DIR}/../Artifacts/"
    )
endif ()