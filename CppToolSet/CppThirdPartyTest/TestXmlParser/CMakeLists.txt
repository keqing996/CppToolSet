cmake_minimum_required(VERSION 3.26)

project (TestXmlParser)

set(CMAKE_CXX_STANDARD 20)

file (GLOB_RECURSE src_files src/*.cpp)

add_executable(test_xml_parser ${src_files})

target_include_directories(test_xml_parser
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/src/ # library
)

add_custom_command(
        TARGET test_xml_parser POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/config/"  "${CMAKE_CURRENT_BINARY_DIR}/config/"
)