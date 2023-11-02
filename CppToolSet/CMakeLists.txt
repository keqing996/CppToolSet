cmake_minimum_required(VERSION 3.26)

project(CppTools)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    add_compile_definitions(BUILD_DEBUG)
endif ()

add_subdirectory(ThirdParty/src/gtest)
add_subdirectory(CppUtils)
add_subdirectory(CppUiProgramTemplate)
add_subdirectory(CppToolVsNewProjWin32Deleter)
add_subdirectory(CppToolFileToCharArray)
# add_subdirectory(CppGraphics)


