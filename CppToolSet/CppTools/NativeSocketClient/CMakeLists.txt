cmake_minimum_required(VERSION 3.26)

project(NativeSocketClient)

set(TargetName socket_client)

set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(_UNICODE UNICODE) # windows wchar
add_compile_definitions(NOMINMAX) # windows
add_compile_definitions(WIN32_LEAN_AND_MEAN) # windows socket

file (GLOB_RECURSE src_files src/*.cpp)
file (GLOB_RECURSE lib_files_d ${CMAKE_CURRENT_SOURCE_DIR}/../../Artifacts/Library/WindowsApi/lib/*_d*.lib)
file (GLOB_RECURSE lib_files ${CMAKE_CURRENT_SOURCE_DIR}/../../Artifacts/Library/WindowsApi/lib/*.lib)

add_executable(${TargetName} ${src_files})

target_include_directories(${TargetName}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/src/ # cmdline
        ${CMAKE_CURRENT_SOURCE_DIR}/../../Artifacts/Library/WindowsApi/include/ # win socket
)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_link_libraries(${TargetName} ${lib_files_d} )
else()
    target_link_libraries(${TargetName} ${lib_files} )
endif ()

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