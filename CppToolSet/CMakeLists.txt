cmake_minimum_required(VERSION 3.26)

project(CppTools)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    add_compile_definitions(BUILD_DEBUG)
endif ()

set(UI_TEMPLATE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CppUiProgramTemplate)

add_subdirectory(CppUiProgramTemplate)
add_subdirectory(CppToolVsNewProjWin32Deleter)
add_subdirectory(CppToolFileToCharArray)
add_subdirectory(CppToolMdNoUseImgDeleter)
add_subdirectory(CppToolHexoMarkDownTool)
add_subdirectory(CppToolNativeSocketClient)
add_subdirectory(CppToolQuickOpenFolder)


