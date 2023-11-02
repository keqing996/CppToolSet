cmake_minimum_required(VERSION 3.26)

project(CppUtils)

set(TargetName test_util)

enable_testing()

file (GLOB_RECURSE test_Files test/*.cpp)

add_executable(${TargetName}
        ${test_Files}
)

target_include_directories(${TargetName}
        PRIVATE
        ${gtest_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR}
)

target_link_libraries(${TargetName}
        gtest
        gtest_main
)

add_test(NAME test_util COMMAND test_util)