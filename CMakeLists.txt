cmake_minimum_required (VERSION 3.22)

project (CppToolSet)

set (CMAKE_CXX_STANDARD 20)
set (EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Artifacts/${CMAKE_CXX_COMPILER_ID}/${CMAKE_BUILD_TYPE}/)

set (INFRA_INCLUDE_PATH         ./ThirdParty/Infra/include/)
set (NLOHMANN_JSON_INCLUDE_PATH ./ThirdParty/NlohmannJson/include)
set (RAPID_XML_INCLUDE_PATH     ./ThirdParty/RapidXml/include)

# Tool File To Char Array
add_executable              (file_to_char_array ./ToolFileToCharArray/Main.cpp)
target_include_directories  (file_to_char_array PRIVATE ${INFRA_INCLUDE_PATH})

# Tool VS Win32 Deleter
add_executable              (vs_win32_deleter ./ToolVsWin32Deleter/Main.cpp)
target_include_directories  (vs_win32_deleter PRIVATE ${RAPID_XML_INCLUDE_PATH})

# Tool Markdown No Use Image Deleter
add_executable              (md_no_use_img_deleter ./ToolMarkdownNoUseImgDeleter/Main.cpp)
target_include_directories  (md_no_use_img_deleter PRIVATE ${INFRA_INCLUDE_PATH})

# Tool Hexo Markdown Converter
add_executable              (hexo_md_converter ./ToolHexoMdConverter/Main.cpp)
target_include_directories  (hexo_md_converter PRIVATE ${INFRA_INCLUDE_PATH})
