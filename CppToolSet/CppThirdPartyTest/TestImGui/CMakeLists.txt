cmake_minimum_required(VERSION 3.26)

project (ImGuiTemplate)

set(TargetName im_gui_template)

set(CMAKE_CXX_STANDARD 20)

set(IM_GUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/src/imgui/)

file (GLOB_RECURSE src_files src/*.cpp)

add_executable(${TargetName}
        ${src_files}
        ${IM_GUI_DIR}/backends/imgui_impl_dx11.cpp
        ${IM_GUI_DIR}/backends/imgui_impl_win32.cpp
        ${IM_GUI_DIR}/imgui.cpp
        ${IM_GUI_DIR}/imgui_draw.cpp
        ${IM_GUI_DIR}/imgui_tables.cpp
        ${IM_GUI_DIR}/imgui_widgets.cpp
)

target_include_directories(${TargetName}
        PRIVATE
        ${IM_GUI_DIR}
        ${IM_GUI_DIR}/backends
)

find_library(D3D11_LIBRARY d3d11.lib
        PATHS
        "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/")

target_link_libraries(${TargetName} PRIVATE d3d11.lib)

