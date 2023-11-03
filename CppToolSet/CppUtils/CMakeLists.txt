cmake_minimum_required(VERSION 3.26)

project(CppUtils)

set(TargetName test_util)
set(CMAKE_CXX_STANDARD 20)

enable_testing()

file (GLOB_RECURSE src_files src/*.cpp)
file (GLOB_RECURSE test_files test/*.cpp)

add_executable(${TargetName}
        ${src_files}
        ${test_files}
)

target_include_directories(${TargetName}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ # self root
        ${gtest_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR}
)

target_link_libraries(${TargetName}
        gtest
        gtest_main
)

add_test(NAME ${TargetName} COMMAND util_gtest)