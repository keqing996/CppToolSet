
#include <fstream>
#include <filesystem>
#include <format>
#include <iostream>
#include <cmdline/cmdline.h>

int main(int argc, char** argv)
{
    CmdLine::Parser cmd;

    cmd.Add<std::string>("input", 'i', "input file", true, "");
    cmd.Add<std::string>("name", 'n', "var name", false);

    cmd.ParseCheck(argc, argv);

    std::string inputPath = cmd.Get<std::string>("input");
    std::string varName = "data";
    if (cmd.Exist("name"))
        varName = cmd.Get<std::string>("name");

    if (!std::filesystem::exists(inputPath))
    {
        std::cout << "file not exist." << std::endl;
        return 1;
    }

    if (std::filesystem::is_directory(inputPath))
    {
        std::cout << "target is a directory." << std::endl;
        return 1;
    }

    std::filesystem::path inputFile(inputPath);
    std::string outFileName = inputFile.stem().string() + ".h";
    std::filesystem::path outputFile = inputFile.parent_path().append(outFileName);

    // get input
    std::ifstream inputStream(inputFile, std::ios::in | std::ios::binary);
    if (!inputStream.is_open())
    {
        std::cout << "input file open failed." << std::endl;
        return 1;
    }

    inputStream.seekg(0, std::ios::end);
    unsigned int size = inputStream.tellg();

    std::unique_ptr<char[]> content(new char[size]);

    inputStream.seekg(0, std::ios::beg);
    inputStream.read(&content[0], size);
    inputStream.close();

    // output
    std::ofstream outputStream(outputFile, std::ios::out);
    if (!outputStream.is_open())
    {
        std::cout << "output file open failed." << std::endl;
        return 1;
    }

    outputStream << "#pragma once\n";
    outputStream << "\n";
    outputStream << "// Auto Generated File.\n";
    outputStream << std::format("// Generated from: {}\n", inputFile.filename().string());
    outputStream << "\n";
    outputStream << "#include <array>\n";
    outputStream << "\n";
    outputStream << std::format("inline static std::array<unsigned char, {}> {} = \n", size, varName);
    outputStream << "{";

    for (int i = 0; i < size - 1; i++)
    {
        if (i % 15 == 0)
            outputStream << std::format("\n\t0x{:02X}, ", static_cast<unsigned char>(content[i]));
        else
            outputStream << std::format("0x{:02X}, ", static_cast<unsigned char>(content[i]));
    }

    outputStream << "\n};\n";
}