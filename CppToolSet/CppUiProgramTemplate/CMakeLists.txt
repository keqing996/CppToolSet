cmake_minimum_required(VERSION 3.26)

project(CppUiTemplate)

set(TargetName ui_template)
set(IM_GUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/src/imgui/)
set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

file (GLOB_RECURSE res_files resource/*.rc)
file (GLOB_RECURSE src_files src/*.cpp)
file (GLOB_RECURSE manifest_files manifest/*.manifest)

file (GLOB_RECURSE lib_files_d ${CMAKE_CURRENT_SOURCE_DIR}/../Artifacts/Library/WindowsApi/lib/*_d*.lib)
file (GLOB_RECURSE lib_files ${CMAKE_CURRENT_SOURCE_DIR}/../Artifacts/Library/WindowsApi/lib/*.lib)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    add_compile_definitions(BUILD_DEBUG)
endif ()

add_executable(${TargetName}
        ${src_files}
        ${res_files}
        ${manifest_files}
        ${IM_GUI_DIR}/backends/imgui_impl_dx11.cpp
        ${IM_GUI_DIR}/backends/imgui_impl_opengl3.cpp
        ${IM_GUI_DIR}/backends/imgui_impl_win32.cpp
        ${IM_GUI_DIR}/imgui.cpp
        ${IM_GUI_DIR}/imgui_draw.cpp
        ${IM_GUI_DIR}/imgui_tables.cpp
        ${IM_GUI_DIR}/imgui_widgets.cpp
)

set_target_properties(${TargetName} PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:WINDOWS"
)

target_include_directories(${TargetName}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ # self root
        ${CMAKE_CURRENT_SOURCE_DIR}/../Artifacts/Library/StringUtil/ # string util
        ${CMAKE_CURRENT_SOURCE_DIR}/../Artifacts/Library/WindowsApi/include/ # windows api
        ${IM_GUI_DIR}   # imgui
        ${IM_GUI_DIR}/backends # imgui backend
)

find_library(D3D11_LIBRARY d3d11.lib
        PATHS
        "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/"
)

target_link_libraries(${TargetName}
        d3d11.lib # d3d
)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_link_libraries(${TargetName} ${lib_files_d} )
else()
    target_link_libraries(${TargetName} ${lib_files} )
endif ()