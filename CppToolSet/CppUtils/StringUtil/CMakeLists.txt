cmake_minimum_required(VERSION 3.26)

project(StringUtil)

set(TargetName string_util)

set(CMAKE_CXX_STANDARD 20)

file (GLOB_RECURSE src_files test/*.cpp)

add_executable(${TargetName} ${src_files} ${res_files})

# target_link_libraries(${TargetName}
#         gtest
#         gtest_main
# )

# add_test(NAME test COMMAND ${TargetName})

# copy hpp
add_custom_command(
        TARGET ${TargetName} POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/include/"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../Artifacts/Library/include/Util/"
)