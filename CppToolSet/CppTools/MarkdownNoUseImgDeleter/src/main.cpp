
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

        std::filesystem::path path = f.path();
        std::filesystem::path ext = path.extension();

        if (ext == ".md")
            _allMdFileSet.emplace(path.string());
        else if (ext == ".jpg" || ext == ".jpeg" || ext == ".png" || ext == ".svg")
            _allImgSet.emplace(path.string());




        std::cout << ext << std::endl;
    }

    return 0;
}