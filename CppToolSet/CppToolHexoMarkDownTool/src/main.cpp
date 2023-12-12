
#include <filesystem>
#include <iostream>
#include <string>

#include "ConsoleLogger.hpp"
#include "Processor.hpp"

int main()
{
    InitConsoleLogger();

    std::wcout.imbue(std::locale {"zh_CN"});

    auto currPath = std::filesystem::current_path();
    ProcessDirectory(currPath);

    system("pause");

    return 0;
}