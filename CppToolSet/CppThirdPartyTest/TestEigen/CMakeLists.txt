cmake_minimum_required(VERSION 3.26)

project (TestEigen)

set(CMAKE_CXX_STANDARD 20)

file (GLOB_RECURSE src_files src/*.cpp)

add_executable(test_eigen ${src_files})

target_include_directories(test_eigen
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/src/ # library
)