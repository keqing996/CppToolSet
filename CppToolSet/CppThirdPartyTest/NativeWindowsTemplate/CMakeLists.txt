cmake_minimum_required(VERSION 3.26)

project(NativeWindowsTemplate)

set(TargetName native_window_template)

set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(_UNICODE UNICODE) # windows wchar
add_compile_definitions(NOMINMAX) # cancel windows min max macro

file (GLOB_RECURSE src_files src/*.cpp)

add_executable(${TargetName} ${src_files}
        src/Application/application.cpp
        src/Application/application.h
        src/Application/Application.cpp
        src/Application/Application.h
        src/Input/Keyboard.cpp
        src/Input/Keyboard.h
        src/Input/Mouse.cpp
        src/Input/Mouse.h
        src/Util/Timer.hpp
        src/Util/Util.hpp)

set_target_properties(${TargetName} PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:WINDOWS"
)
