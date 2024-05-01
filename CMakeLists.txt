cmake_minimum_required(VERSION 3.22)

project(CppToolSet)

set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Artifacts/${CMAKE_CXX_COMPILER_ID}/${CMAKE_BUILD_TYPE}/)

set (INFRA_INCLUDE_PATH ./ThirdParty/Infra/include/)

# Tool File To Char Array
add_executable              (file_to_char_array ./ToolFileToCharArray/Main.cpp)
target_include_directories  (file_to_char_array PRIVATE ${INFRA_INCLUDE_PATH})
