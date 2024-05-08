
#include <fstream>
#include <filesystem>
#include <format>
#include <iostream>
#include <thread>
#include <Infra/Windows/Console.hpp>

int main(int argc, char** argv)
{
    // arg[0] = self.exe
    // arg[1] = source
    if (argc != 2)
    {
        std::cout << "Error input arguments";
        return 1;
    }

    std::string inputPath = argv[1];

    if (!std::filesystem::exists(inputPath))
    {
        std::cout << "File not exist." << std::endl;
        return 1;
    }

    if (std::filesystem::is_directory(inputPath))
    {
        std::cout << "Target is a directory." << std::endl;
        return 1;
    }

    std::filesystem::path inputFile(inputPath);
    std::string inputFileName = inputFile.stem().string();
    std::string outFileName = inputFileName + ".h";
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

    // main thread only read, so no lock/atomic
    float progress = 0;
    bool finish = false;
    std::thread worker([&]() -> void
    {
        outputStream << "#pragma once\n";
        outputStream << "\n";
        outputStream << "// Auto Generated File.\n";
        outputStream << std::format("// Generated from: {}\n", inputFile.filename().string());
        outputStream << "\n";
        outputStream << "#include <array>\n";
        outputStream << "\n";
        outputStream << std::format("inline static std::array<unsigned char, {}> {} = \n", size, inputFileName);
        outputStream << "{";

        for (int i = 0; i < size - 1; i++)
        {
            if (i % 15 == 0)
            {
                outputStream << std::format("\n\t0x{:02X}, ", static_cast<unsigned char>(content[i]));
            }
            else
                outputStream << std::format("0x{:02X}, ", static_cast<unsigned char>(content[i]));

            progress = (float) i / (size - 1);
        }

        outputStream << "\n};\n";

        finish = true;
    });

    while (true)
    {
        if (finish)
            break;

        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        Infra::Console::ProgressBar(progress);
    }

    worker.join();

    Infra::Console::ProgressBar(1.0f);

    return 0;
}