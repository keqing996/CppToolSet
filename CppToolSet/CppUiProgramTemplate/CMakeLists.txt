cmake_minimum_required(VERSION 3.26)

project(CppUiTemplate)

set(TargetNameEmbeddedTtf ui_template_embedded_ttf)
set(TargetNameSystemTtf ui_template_system_ttf)

set(IM_GUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/src/imgui/)
set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

file (GLOB_RECURSE UI_TEMPLATE_SRC Framework/*.cpp)
file (GLOB_RECURSE UI_TEMPLATE_RESOURCE Framework/*.rc)
file (GLOB_RECURSE UI_TEMPLATE_MANIFEST ${CMAKE_CURRENT_SOURCE_DIR}/../GeneralResource/manifest/*.manifest)

file (GLOB_RECURSE res_files resource/*.rc)
file (GLOB_RECURSE src_files src/*.cpp)
file (GLOB_RECURSE util_src_files ../Util/*.cpp)

set (ALL_SOURCE
        ${UI_TEMPLATE_SRC}
        ${UI_TEMPLATE_RESOURCE}
        ${UI_TEMPLATE_MANIFEST}
        ${src_files}
        ${util_src_files}
        ${res_files}
        ${manifest_files}
        ${IM_GUI_DIR}/backends/imgui_impl_dx11.cpp
        ${IM_GUI_DIR}/backends/imgui_impl_win32.cpp
        ${IM_GUI_DIR}/imgui.cpp
        ${IM_GUI_DIR}/imgui_draw.cpp
        ${IM_GUI_DIR}/imgui_tables.cpp
        ${IM_GUI_DIR}/imgui_widgets.cpp
)

set (ALL_INCLUDE_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ # self root
        ${CMAKE_CURRENT_SOURCE_DIR}/../Util/ # util
        ${IM_GUI_DIR}   # imgui
        ${IM_GUI_DIR}/backends # imgui backend)
)

find_library(D3D11_LIBRARY d3d11.lib
        PATHS
        "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/"
)

## Embedded TTF

add_executable(${TargetNameEmbeddedTtf} ${ALL_SOURCE})
set_target_properties(${TargetNameEmbeddedTtf} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
target_compile_definitions(${TargetNameEmbeddedTtf} PRIVATE EMBEDDED_TTF)
target_include_directories(${TargetNameEmbeddedTtf} PRIVATE ${ALL_INCLUDE_DIR})
target_link_libraries(${TargetNameEmbeddedTtf} d3d11.lib )

## System TTF

add_executable(${TargetNameSystemTtf} ${ALL_SOURCE})
set_target_properties(${TargetNameSystemTtf} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
target_include_directories(${TargetNameSystemTtf} PRIVATE ${ALL_INCLUDE_DIR})
target_link_libraries(${TargetNameSystemTtf} d3d11.lib )

## Debug macro
if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_compile_definitions(${TargetNameEmbeddedTtf} PRIVATE ENABLE_CONSOLE_LOG)
    target_compile_definitions(${TargetNameSystemTtf} PRIVATE ENABLE_CONSOLE_LOG)
endif ()