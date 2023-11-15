cmake_minimum_required(VERSION 3.26)

project(NativeSocketClient)

set(TargetName tool_socket_client)
set(CMAKE_CXX_STANDARD 20)
set(IM_GUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/src/imgui/)

file (GLOB_RECURSE UI_TEMPLATE_SRC      ${UI_TEMPLATE_PATH}/Framework/*.cpp)
file (GLOB_RECURSE UI_TEMPLATE_RESOURCE ${UI_TEMPLATE_PATH}/Framework/*.rc)
file (GLOB_RECURSE UI_TEMPLATE_MANIFEST ${CMAKE_CURRENT_SOURCE_DIR}/../GeneralResource/manifest/*.manifest)

file (GLOB_RECURSE res_files resource/*.rc)
file (GLOB_RECURSE src_files src/*.cpp)
file (GLOB_RECURSE util_src_files ../CppUtils/src/*.cpp)

add_executable(${TargetName}
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

target_include_directories(${TargetName}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ # self root
        ${CMAKE_CURRENT_SOURCE_DIR}/../CppUtils/include/ # util
        ${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/src/ # json
        ${UI_TEMPLATE_PATH}/ # ui template framework
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

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_compile_definitions(${TargetName} PRIVATE ENABLE_CONSOLE_LOG)
endif ()

# release copy artifacts
if (CMAKE_BUILD_TYPE STREQUAL Release)
    add_custom_command(
            TARGET ${TargetName} POST_BUILD
            COMMAND ${CMAKE_COMMAND}
            -E copy
            $<TARGET_FILE:${TargetName}>
            "${CMAKE_CURRENT_SOURCE_DIR}/../Artifacts/${TargetName}.exe"
    )
endif ()