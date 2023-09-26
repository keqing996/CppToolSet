cmake_minimum_required(VERSION 3.26)

project (WindowsApiTest)

set(TargetName test_windows_api)

set(CMAKE_CXX_STANDARD 20)

file (GLOB_RECURSE src_files src/*.cpp)
file (GLOB_RECURSE lib_files_d ${CMAKE_CURRENT_SOURCE_DIR}/../../Artifacts/Library/WindowsApi/lib/*_d.lib)
file (GLOB_RECURSE lib_files ${CMAKE_CURRENT_SOURCE_DIR}/../../Artifacts/Library/WindowsApi/lib/*.lib)

add_executable(${TargetName} ${src_files})

target_include_directories(${TargetName}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../Artifacts/Library/WindowsApi/include/
)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_link_libraries(${TargetName} ${lib_files_d} )
else()
    target_link_libraries(${TargetName} ${lib_files} )
endif ()