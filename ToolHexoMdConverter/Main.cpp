
#include <filesystem>
#include <iostream>
#include <string>
#include "Processor.hpp"

int main()
{
    std::wcout.imbue(std::locale {"zh_CN"});

    auto currPath = std::filesystem::current_path();
    ProcessDirectory(currPath);

    system("pause");

    return 0;
}