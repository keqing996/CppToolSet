#pragma once

#include <regex>
#include <fstream>
#include <vector>
#include <filesystem>
#include <Infra/String.hpp>
#include "ConsoleLogger.hpp"

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
    std::wstring mdFileNameW = Infra::String::StringToWideString(mdFileName);

    // change resource folder name
    auto targetResPath = mdPath.parent_path() / mdFileName;
    if (std::filesystem::exists(targetResPath))
    {
        LogError(std::format("\tError: {} Already Exists!", targetResPath.string()));
        return;
    }

    std::filesystem::rename(resPath, targetResPath);

    // change md file img path
    wRegex mdImgExpression{L"!\\[(.*)\\]\\((.*)\\)"};

    std::wifstream wideInputStream(mdPath, std::ios::in);

    if (!wideInputStream.is_open())
    {
        LogError(std::format("\tError: {} Read Open Failed!", mdPath.string()));
        return;
    }

    wideInputStream.imbue(std::locale {"zh_CN.UTF-8"});

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

        SetConsoleColor(Infra::Console::Color::Cyan);
        std::wcout << std::format(L"\t<--: {}", oneLineContent) << std::endl;

        std::wstring title = regexResult[1];
        std::wstring imgPath = regexResult[2];
        Infra::String::Replace<wchar_t>(imgPath, L"Resource/", L"");
        oneLineContent = std::format(L"{{% asset_img {} {} %}}", imgPath, title);

        SetConsoleColor(Infra::Console::Color::Green);
        std::wcout << std::format(L"\t-->: {}", oneLineContent) << std::endl;

        SetConsoleColor(Infra::Console::Color::None);

        gFileContentBuffer.push_back(oneLineContent);
    }

    wideInputStream.close();

    // write back

    std::wofstream wideOutputStream(mdPath, std::ios::out | std::ios::trunc);

    if (!wideOutputStream.is_open())
    {
        LogError(std::format("\tError: {} Write Open Failed!", mdPath.string()));
        return;
    }

    wideOutputStream.imbue(std::locale {"zh_CN.UTF-8"});

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
                LogError(std::format(L"Error: Duplicate Md File In One Directory: {}", f.path().wstring()));
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