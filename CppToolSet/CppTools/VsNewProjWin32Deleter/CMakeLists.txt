cmake_minimum_required(VERSION 3.26)

project(VsNewProjWin32Deleter)

set(TargetName vs_sln_win32_deleter)

set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(_UNICODE UNICODE) # windows wchar

file (GLOB_RECURSE src_files src/*.cpp)

add_executable(${TargetName} ${src_files} ${res_files})

target_include_directories(${TargetName}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/src/
)

# copy config
add_custom_command(
        TARGET ${TargetName} POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/testxml/"  "${CMAKE_CURRENT_BINARY_DIR}/testxml/"
)