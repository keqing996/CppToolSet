
#include <filesystem>
#include <iostream>
#include <regex>
#include <unordered_set>
#include <string>

int main()
{
    std::unordered_set<std::string> _allImgSet;
    std::unordered_set<std::string> _allMdFileSet;

    auto currPath = std::filesystem::current_path();
    for (const auto& f: std::filesystem::recursive_directory_iterator(currPath))
    {
        if (f.is_directory())
            continue;

        auto ext = f.path().extension();

        std::cout << ext << std::endl;
    }

    return 0;
}