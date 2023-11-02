cmake_minimum_required(VERSION 3.26)

project (QuickOpenFolder)

set(TargetName quick_open_folder)
set(CMAKE_CXX_STANDARD 20)
set(IM_GUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/src/imgui/)

add_compile_definitions(_UNICODE UNICODE) # windows wchar
add_compile_definitions(NOMINMAX) # cancel windows min max macro

file (GLOB_RECURSE res_files resource/*.rc)
file (GLOB_RECURSE src_files src/*.cpp)
file (GLOB_RECURSE manifest_files manifest/*.manifest)

add_executable(${TargetName}
        ${src_files}
        ${res_files}
        ${manifest_files}
        ${IM_GUI_DIR}/backends/imgui_impl_dx11.cpp
        ${IM_GUI_DIR}/backends/imgui_impl_win32.cpp
        ${IM_GUI_DIR}/imgui.cpp
        ${IM_GUI_DIR}/imgui_draw.cpp
        ${IM_GUI_DIR}/imgui_tables.cpp
        ${IM_GUI_DIR}/imgui_widgets.cpp
)

target_include_directories(${TargetName}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ # self root
        ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/src/ # json
        ${IM_GUI_DIR}
        ${IM_GUI_DIR}/backends
)

set_target_properties(${TargetName} PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:WINDOWS"
)

find_library(D3D11_LIBRARY d3d11.lib
        PATHS
        "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/"
)

target_link_libraries(${TargetName} PRIVATE d3d11.lib)