#pragma once

#include <string>
#include <vector>

namespace WinApi::FileDialog
{
    struct FileTypeFilter
    {
        std::wstring name;
        std::wstring suffix;
    };

    /*
     * example { { L"Text Files", L"*.txt" }, { L"All Files", L"*.*" } }
     */
    std::wstring OpenFile(const std::wstring& titleMsg, const std::vector<FileTypeFilter>* pFilter = nullptr);

    std::wstring OpenDirectory(const std::wstring& titleMsg);
}