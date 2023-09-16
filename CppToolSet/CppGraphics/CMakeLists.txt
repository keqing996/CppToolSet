cmake_minimum_required(VERSION 3.26)

project(CppGraphics)

set(TargetName renderer)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

add_compile_definitions(_UNICODE UNICODE) # windows wchar
add_compile_definitions(NOMINMAX) # cancel windows min max macro

file (GLOB_RECURSE res_files resource/*.rc)
file (GLOB_RECURSE src_files src/*.cpp)

add_executable(${TargetName} ${src_files} ${res_files})

target_include_directories(${TargetName}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ # self root
        ${OPENGL_INCLUDE_DIR} # openGL
)

target_link_libraries(${TargetName}
        ${OPENGL_LIBRARIES} # openGL
)

set_target_properties(${TargetName} PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:WINDOWS"
)