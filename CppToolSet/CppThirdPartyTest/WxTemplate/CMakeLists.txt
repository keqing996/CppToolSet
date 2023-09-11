cmake_minimum_required(VERSION 3.26)

project (WxTemplate)

set(TargetName wx_template)

set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(_UNICODE) # wxWidget need _UNICODE

file (GLOB_RECURSE src_files src/*.cpp)
file (GLOB_RECURSE res_files resource/*.rc)
file (GLOB_RECURSE lib_files_d ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/lib/wxWidgets/lib/vc_x64_lib/*d*.lib)
file (GLOB_RECURSE lib_files ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/lib/wxWidgets/lib/vc_x64_lib/*.lib)

add_executable(${TargetName} ${src_files} ${res_files})

set_target_properties(${TargetName} PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:WINDOWS"
)

target_include_directories(${TargetName}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/lib/wxWidgets/include/
        ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/lib/wxWidgets/include/msvc/
)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_link_libraries(${TargetName} ${lib_files_d} )
else()
    target_link_libraries(${TargetName} ${lib_files} )
endif ()

