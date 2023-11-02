cmake_minimum_required(VERSION 3.26)

project(NativeSocketClient)

set(TargetName tool_socket_client)

set(CMAKE_CXX_STANDARD 20)

file (GLOB_RECURSE src_files src/*.cpp)
file (GLOB_RECURSE util_src_files ../CppUtils/src/*.cpp)

add_executable(${TargetName} ${src_files} ${util_src_files})

target_include_directories(${TargetName}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/src/ # cmdline
        ${CMAKE_CURRENT_SOURCE_DIR}/../CppUtils/include/ # util
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