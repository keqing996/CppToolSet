cmake_minimum_required(VERSION 3.26)

project(CppGraphics)

set(TargetName renderer)
set(IM_GUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/src/imgui-1.89/)
set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

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
        ${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/src/ # eigen
        ${IM_GUI_DIR}   # imgui
        ${IM_GUI_DIR}/backends # imgui backend
)

find_library(D3D11_LIBRARY d3d11.lib
        PATHS
        "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/"
)

target_link_libraries(${TargetName}
        ${OPENGL_LIBRARIES} # opengl import lib -> Windows Kits\10\Lib\10.0.xxx.0\um\x64\OpenGL32.Lib
        d3d11.lib # d3d
)