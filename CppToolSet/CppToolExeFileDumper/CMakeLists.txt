cmake_minimum_required(VERSION 3.26)

project(PeFileDumper)

set(TargetName pe_file_dumper)

set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(_UNICODE UNICODE) # windows wchar

file (GLOB_RECURSE src_files src/*.cpp)

add_executable(${TargetName} ${src_files} ${res_files})

# release copy artifacts
if (CMAKE_BUILD_TYPE STREQUAL Release)
    add_custom_command(
            TARGET ${TargetName} POST_BUILD
            COMMAND ${CMAKE_COMMAND}
            -E copy
            $<TARGET_FILE:${TargetName}>
            "${CMAKE_CURRENT_SOURCE_DIR}/../../Artifacts/Tool/${TargetName}.exe"
    )
endif ()