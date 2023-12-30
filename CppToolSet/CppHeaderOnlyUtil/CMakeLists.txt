cmake_minimum_required(VERSION 3.26)

project (HeadOnlyUtil)

set(TargetName util_tester)
set(CMAKE_CXX_STANDARD 20)

file (GLOB_RECURSE src_files src/*.cpp)

add_executable(${TargetName}
        ${src_files}
)

target_include_directories(${TargetName}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
)