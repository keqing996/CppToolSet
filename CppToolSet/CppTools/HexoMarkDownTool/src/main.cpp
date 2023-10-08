
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_set>
#include <string>
#include <format>
#include "StringUtil.hpp"

using wRegex = std::basic_regex<wchar_t>;

std::vector<std::wstring> gFileContentBuffer;

void ProcessMarkdownFile(const std::filesystem::path& mdPath)
{
    std::cout << std::format("Processing: {}", mdPath.string()) << std::endl;

    // find resource
    auto resPath = mdPath.parent_path() / "Resource";
    if (!std::filesystem::exists(resPath))
    {
        std::cout << "\tNo Resource Directory, Finished." << std::endl << std::endl;
        return;
    }

    // abstract file name
    std::string mdFileName = mdPath.filename().stem().string();
    std::wstring mdFileNameW = StringUtil::StringToWString(mdFileName);

    // change resource folder name
    auto targetResPath = mdPath.parent_path() / mdFileName;
    if (std::filesystem::exists(targetResPath))
    {
        std::cout << std::format("\tError: {} Already Exists!", targetResPath.string()) << std::endl;
        return;
    }

    std::filesystem::rename(resPath, targetResPath);

    // change md file img path
    wRegex mdImgExpression{L"!\\[(.*)\\]\\((.*)\\)"};

    std::wifstream wideInputStream(mdPath, std::ios::in);

    if (!wideInputStream.is_open())
    {
        std::cout << std::format("\tError: {} Read Open Failed!", mdPath.string()) << std::endl;
        return;
    }

    std::wstring oneLineContent;
    std::wsmatch regexResult;

    gFileContentBuffer.clear();
    while (std::getline(wideInputStream, oneLineContent))
    {
        if (!std::regex_match(oneLineContent, regexResult, mdImgExpression))
        {
            gFileContentBuffer.push_back(oneLineContent);
            continue;
        }

        std::wcout << std::format(L"\t<--: {}", oneLineContent) << std::endl;

        std::wstring title = regexResult[1];
        std::wstring imgPath = StringUtil::Replace<wchar_t>(regexResult[2], L"Resource/", L"");
        oneLineContent = std::format(L"{{% asset_img {} {} %}}", imgPath, title);
        std::wcout << std::format(L"\t-->: {}", oneLineContent) << std::endl;
        gFileContentBuffer.push_back(oneLineContent);
    }

    wideInputStream.close();

    // write back

    std::wofstream wideOutputStream(mdPath, std::ios::out | std::ios::trunc);

    if (!wideOutputStream.is_open())
    {
        std::cout << std::format("\tError: {} Write Open Failed!", mdPath.string()) << std::endl;
        return;
    }

    for (const auto& line : gFileContentBuffer)
        wideOutputStream << line << std::endl;

    wideOutputStream.close();

    gFileContentBuffer.clear();

    std::cout << std::endl;
}

void ProcessDirectory(const std::filesystem::path& path)
{
    if (!std::filesystem::exists(path))
        return;

    if (!std::filesystem::is_directory(path))
        return;

    std::vector<std::filesystem::path> nextDirectories;
    std::optional<std::filesystem::path> thisDirectoryMdFile;
    for (const auto& f: std::filesystem::directory_iterator(path))
    {
        if (f.is_directory())
        {
            nextDirectories.push_back(f.path());
            continue;
        }

        std::filesystem::path ext = f.path().extension();
        if (ext == ".md")
        {
            if (thisDirectoryMdFile.has_value())
            {
                std::wcout << std::format(L"Error: Duplicate Md File In One Directory: {}", f.path().wstring());
                continue;
            }
            else
                thisDirectoryMdFile = f.path();
        }
    }

    for (const auto& p : nextDirectories)
        ProcessDirectory(p);

    if (thisDirectoryMdFile.has_value())
        ProcessMarkdownFile(thisDirectoryMdFile.value());
}

int main()
{
    auto currPath = std::filesystem::current_path();
    ProcessDirectory(currPath);

    system("pause");

    return 0;
}