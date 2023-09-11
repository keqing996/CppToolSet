cmake_minimum_required(VERSION 3.26)

project (WxWidgetQuickOpenFolder)

set(TargetName quick_open_folder)

set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(_UNICODE) # wxWidget need _UNICODE
add_compile_definitions(UNICODE) # for windows wchar

file (GLOB_RECURSE src_files src/*.cpp)
file (GLOB_RECURSE res_files resource/resource.*)
file (GLOB_RECURSE lib_files_d ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/lib/wxWidgets/lib/vc_x64_lib/*d*.lib)
file (GLOB_RECURSE lib_files ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/lib/wxWidgets/lib/vc_x64_lib/*.lib)

add_executable(${TargetName} ${src_files} ${res_files})

set_target_properties(${TargetName} PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:WINDOWS"
)

target_include_directories(${TargetName}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/src/ # json
        ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/lib/wxWidgets/include/
        ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/lib/wxWidgets/include/msvc/
)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_link_libraries(${TargetName} ${lib_files_d} )
else()
    target_link_libraries(${TargetName} ${lib_files} )
endif ()

# copy config
add_custom_command(
        TARGET ${TargetName} POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/config/"  "${CMAKE_CURRENT_BINARY_DIR}/"
)

# release copy artifacts
if (CMAKE_BUILD_TYPE STREQUAL Release)
    add_custom_command(
            TARGET ${TargetName} POST_BUILD
            COMMAND ${CMAKE_COMMAND}
            -E copy
            $<TARGET_FILE:${TargetName}>
            "${CMAKE_CURRENT_SOURCE_DIR}/../../Artifacts/Tool/QuickOpenFolder/${TargetName}.exe"
    )
    add_custom_command(
            TARGET ${TargetName} POST_BUILD
            COMMAND ${CMAKE_COMMAND}
            -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/config/"
            "${CMAKE_CURRENT_SOURCE_DIR}/../../Artifacts/Tool/QuickOpenFolder/config/"
    )
endif ()

