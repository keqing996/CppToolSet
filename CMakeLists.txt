cmake_minimum_required (VERSION 3.22)

project (CppToolSet)

set (CMAKE_CXX_STANDARD 20)
#set (EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Artifacts/${CMAKE_CXX_COMPILER_ID}/${CMAKE_BUILD_TYPE}/)
set (EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Artifacts/${CMAKE_BUILD_TYPE}/)

set (INFRA_INCLUDE_PATH         ./SubModule/Infra/src/)
set (NLOHMANN_JSON_INCLUDE_PATH ./ThirdParty/NlohmannJson/include)
set (RAPID_XML_INCLUDE_PATH     ./ThirdParty/RapidXml/include)

add_subdirectory(SubModule/Infra)

# Tool CMake Quick Start
add_executable              (cmake_quick_start ./ToolCMakeQuickStart/Main.cpp)
target_include_directories  (cmake_quick_start PRIVATE ${INFRA_INCLUDE_PATH})

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

# Tool Quick Open Folder
add_executable              (quick_open_folder  ./ToolQuickOpenFolder/Main.cpp
                                                ./ToolQuickOpenFolder/QuickOpenFolder.cpp
                                                ./ToolQuickOpenFolder/Resource/BundledResource.rc)
target_include_directories  (quick_open_folder PRIVATE ./ThirdParty/NlohmannJson/include/)
target_link_libraries       (quick_open_folder PRIVATE imgui_frame_work)
set_target_properties       (quick_open_folder PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")

# Tool Socket Client
add_executable              (socket_client  ./ToolSocketClient/Main.cpp
                                            ./ToolSocketClient/TargetInputWindow.cpp
                                            ./ToolSocketClient/SocketWindow.cpp
                                            ./ToolSocketClient/Resource/BundledResource.rc)
target_link_libraries       (socket_client PRIVATE imgui_frame_work)
set_target_properties       (socket_client PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
