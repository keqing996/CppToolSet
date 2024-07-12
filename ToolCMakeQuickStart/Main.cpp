#include <iostream>
#include <format>
#include <fstream>
#include <filesystem>
#include <Infra/System.hpp>

int main()
{
    std::string currentDirectory = Infra::System::GetCurrentDirectory();
    std::filesystem::path workDirectory(currentDirectory);

    // create src folder
    std::filesystem::create_directory(workDirectory / L"src");

    // create CMakeList.txt
    std::filesystem::path cmakeListsPath = workDirectory / L"CMakeLists.txt";
    std::ofstream cmakeListsFileStream(cmakeListsPath);
    if (!cmakeListsFileStream.is_open())
    {
        std::wcout << std::format(L"Failed to create file: {}", cmakeListsPath.c_str());
        return 1;
    }

    cmakeListsFileStream << "cmake_minimum_required(VERSION 3.22)" << std::endl;
    cmakeListsFileStream << "set(CMAKE_CXX_STANDARD 20)" << std::endl;
    cmakeListsFileStream << std::endl;
    cmakeListsFileStream << "# name the project" << std::endl;
    cmakeListsFileStream << "project(CMakeQuickStart)" << std::endl;
    cmakeListsFileStream << "set(TARGET_NAME hello_world)" << std::endl;
    cmakeListsFileStream << std::endl;
    cmakeListsFileStream << "# build the executable" << std::endl;
    cmakeListsFileStream << "file(GLOB_RECURSE SRC_FILES src/*.cpp)" << std::endl;
    cmakeListsFileStream << "add_executable(${TARGET_NAME}" << std::endl;
    cmakeListsFileStream << "\t${SRC_FILES}" << std::endl;
    cmakeListsFileStream << ")" << std::endl;
    cmakeListsFileStream << std::endl;
    cmakeListsFileStream << "# exe output path" << std::endl;
    cmakeListsFileStream << "set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/artifacts/)" << std::endl;
    cmakeListsFileStream << std::endl;
    cmakeListsFileStream << "# additional macro" << std::endl;
    cmakeListsFileStream << "# target_compile_definitions(${TARGET_NAME} PRIVATE SOME_MACRO) " << std::endl;
    cmakeListsFileStream << std::endl;
    cmakeListsFileStream << "# additional include directories" << std::endl;
    cmakeListsFileStream << "target_include_directories(${TARGET_NAME} PRIVATE" << std::endl;
    cmakeListsFileStream << "\t${CMAKE_CURRENT_SOURCE_DIR}/src/" << std::endl;
    cmakeListsFileStream << ")" << std::endl;
    cmakeListsFileStream << std::endl;
    cmakeListsFileStream << "# additional static lib directories" << std::endl;
    cmakeListsFileStream << "# target_link_directories(${TARGET_NAME} PRIVATE SOME_LIB)" << std::endl;
    cmakeListsFileStream << std::endl;
    cmakeListsFileStream << "# post build event" << std::endl;
    cmakeListsFileStream << "add_custom_command(TARGET ${TARGET_NAME} POST_BUILD" << std::endl;
    cmakeListsFileStream << "\tCOMMAND echo All Finish." << std::endl;
    cmakeListsFileStream << "\tWORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}" << std::endl;
    cmakeListsFileStream << ")" << std::endl;
    cmakeListsFileStream << std::endl;

    cmakeListsFileStream.close();

    // create main.cpp
    std::filesystem::path mainCppPath = workDirectory / L"src" / L"Main.cpp";
    std::ofstream cppFileStream(mainCppPath);
    if (!cppFileStream.is_open())
    {
        std::wcout << std::format(L"Failed to create file: {}", mainCppPath.c_str());
        return 1;
    }

    cppFileStream << "#include <iostream>" << std::endl;
    cppFileStream << std::endl;
    cppFileStream << "int main()" << std::endl;
    cppFileStream << "{" << std::endl;
    cppFileStream << "\tstd::cout << \"Hello world!\" << std::endl;" << std::endl;
    cppFileStream << "\treturn 0;" << std::endl;
    cppFileStream << "}" << std::endl;
    cppFileStream << std::endl;

    cppFileStream.close();

    // create vscode config
    std::filesystem::path vsCfgPath = workDirectory / L"VscSetting.json";
    std::ofstream vsCfgFileStream(vsCfgPath);
    if (!vsCfgFileStream.is_open())
    {
        std::wcout << std::format(L"Failed to create file: {}", vsCfgPath.c_str());
        return 1;
    }

    vsCfgFileStream << "{" << std::endl;
    vsCfgFileStream << "\t\"cmake.generator\": \"Ninja\"," << std::endl;
    vsCfgFileStream << "\t\"cmake.configureArgs\": [" << std::endl;
    vsCfgFileStream << "\t\t\"-DCMAKE_MAKE_PROGRAM=path/to/ninja/ninja.exe\"," << std::endl;
    vsCfgFileStream << "\t\t\"-DCMAKE_C_COMPILER=path/to/llvm/bin/clang.exe\"," << std::endl;
    vsCfgFileStream << "\t\t\"-DCMAKE_CXX_COMPILER=path/to/llvm/bin/clang++.exe\"" << std::endl;
    vsCfgFileStream << "\t]" << std::endl;
    vsCfgFileStream << "}" << std::endl;

    vsCfgFileStream.close();

    // create cmake bat file
    std::filesystem::path cmakeConfigureBatPath = workDirectory / L"CmakeConfigure.bat";
    std::ofstream cmakeConfigureBatFileStream(cmakeConfigureBatPath);
    if (!cmakeConfigureBatFileStream.is_open())
    {
        std::wcout << std::format(L"Failed to create file: {}", cmakeConfigureBatPath.c_str());
        return 1;
    }

    cmakeConfigureBatFileStream << "cmake -G Ninja -S . -B build ";
    cmakeConfigureBatFileStream << "-DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE ";
    cmakeConfigureBatFileStream << "-DCMAKE_MAKE_PROGRAM=path/to/ninja/ninja.exe ";
    cmakeConfigureBatFileStream << "-DCMAKE_C_COMPILER=path/to/llvm/bin/clang.exe ";
    cmakeConfigureBatFileStream << "-DCMAKE_CXX_COMPILER=path/to/llvm/bin/clang++.exe";
    cmakeConfigureBatFileStream << std::endl;

    cmakeConfigureBatFileStream.close();

    std::filesystem::path cmakeBuildBatPath = workDirectory / L"CmakeBuild.bat";
    std::ofstream cmakeBuildBatFileStream(cmakeBuildBatPath);
    if (!cmakeBuildBatFileStream.is_open())
    {
        std::wcout << std::format(L"Failed to create file: {}", cmakeBuildBatPath.c_str());
        return 1;
    }

    cmakeBuildBatFileStream << "cmake --build \"./build/\"" << std::endl;

    cmakeBuildBatFileStream.close();

    return 0;
}
