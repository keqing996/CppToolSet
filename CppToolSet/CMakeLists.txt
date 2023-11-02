cmake_minimum_required(VERSION 3.26)

project(CppTools)

add_subdirectory(ThirdParty/src/gtest)

enable_testing()

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_subdirectory(CppGraphics)
add_subdirectory(CppThirdPartyTest)
add_subdirectory(CppTools)
add_subdirectory(CppUiProgramTemplate)
add_subdirectory(CppUtils)
