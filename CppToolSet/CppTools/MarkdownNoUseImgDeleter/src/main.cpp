
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_set>
#include <string>
#include <format>

int main()
{
    using wRegex = std::basic_regex<wchar_t>;

    std::unordered_set<std::wstring> _allImgSet;
    std::unordered_set<std::wstring> _allMdFileSet;

    auto currPath = std::filesystem::current_path();
    for (const auto& f: std::filesystem::recursive_directory_iterator(currPath))
    {
        if (f.is_directory())
            continue;

        std::filesystem::path path = f.path();
        std::filesystem::path ext = path.extension();

        if (ext == ".md")
            _allMdFileSet.emplace(path.wstring());
        else if (ext == ".jpg" || ext == ".jpeg" || ext == ".png" || ext == ".svg")
            _allImgSet.emplace(path.wstring());
    }

    wRegex mdImgExpression{L"!\\[(.*)\\]\\((.*)\\)"};

    // all markdown file
    for(const auto& mdFilePath: _allMdFileSet)
    {
        std::wifstream wideInputStream(mdFilePath, std::ios::in);

        if (!wideInputStream.is_open())
        {
            std::wcout << std::format(L"File Not Open: {}", mdFilePath) << std::endl;
            continue;
        }

        std::wstring oneLineContent;
        std::wsmatch regexResult;
        int lineNumber = 0;
        while (std::getline(wideInputStream, oneLineContent))
        {
            lineNumber++;

            if (!std::regex_match(oneLineContent, regexResult, mdImgExpression))
                continue;

            std::wcout << regexResult[2] << std::endl;
        }

    }

    system("pause");

    return 0;
}