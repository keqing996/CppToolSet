
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_set>
#include <string>
#include <format>

#include <Helper/String.h>

int main()
{
    using wRegex = std::basic_regex<wchar_t>;

    std::unordered_set<std::filesystem::path> _allImgSet;
    std::unordered_set<std::filesystem::path> _allMdFileSet;

    auto currPath = std::filesystem::current_path();
    for (const auto& f: std::filesystem::recursive_directory_iterator(currPath))
    {
        if (f.is_directory())
            continue;

        std::filesystem::path path = f.path();
        std::filesystem::path ext = path.extension();

        if (ext == ".md")
            _allMdFileSet.emplace(path);
        else if (ext == ".jpg" || ext == ".jpeg" || ext == ".png" || ext == ".svg")
            _allImgSet.emplace(path);
    }

    std::unordered_set<std::filesystem::path> findFailImgPathSet;
    std::unordered_set<std::filesystem::path> findSuccessImgPathSet;
    wRegex mdImgExpression{L"!\\[(.*)\\]\\((.*)\\)"};

    // all markdown file
    for(const auto& mdFilePath: _allMdFileSet)
    {
        std::wifstream wideInputStream(mdFilePath, std::ios::in);

        if (!wideInputStream.is_open())
        {
            std::wcout << std::format(L"File Not Open: {}", mdFilePath.wstring()) << std::endl;
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

            std::wstring imgPathStr = currPath.wstring() + L"\\" + regexResult[2].str();
            Helper::String::Replace<wchar_t>(imgPathStr, L"%20", L" ");
            std::filesystem::path imgPath {imgPathStr};

            if (_allImgSet.contains(imgPath))
                findSuccessImgPathSet.insert(imgPath);
            else
                findFailImgPathSet.insert(imgPath);
        }
    }

    std::unordered_set<std::filesystem::path> toDeletedPathSet;

    for (const auto& path : _allImgSet)
    {
        if (!findSuccessImgPathSet.contains(path))
            toDeletedPathSet.insert(path);
    }

    if (toDeletedPathSet.empty() && findFailImgPathSet.empty())
    {
        std::wcout << "No File Need To Be Deleted" << std::endl;
        system("pause");
        return 0;
    }

    std::wcout << L"Going Delete Img:\n";
    for (const auto& path : toDeletedPathSet)
        std::wcout << std::format(L"\t{}\n", path.wstring());

    std::wcout << std::endl;
    std::wcout << std::endl;

    std::wcout << L"Not Found Img:\n";
    for (const auto& path : findFailImgPathSet)
        std::wcout << std::format(L"\t{}\n", path.wstring());

    std::wcout << std::endl;

    std::wcout << L"Delete? (Y/N) :\n";

    wchar_t answer;
    std::wcin >> answer;

    if (answer != L'Y')
    {
        system("pause");
        return 0;
    }

    for (const auto& path : toDeletedPathSet)
        std::filesystem::remove(path);

    std::wcout << "Delete Finish !" << std::endl;
    system("pause");

    return 0;
}