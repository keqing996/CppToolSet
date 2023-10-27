#pragma once

#include <string>

namespace WinApi::Encode
{
    std::string WideStringEncodeCharString(const wchar_t* wStr);

    std::string WideStringEncodeCharString(const std::wstring& wStr);

    std::string WideStringEncodeCharString(const std::wstring_view& wStr);


    std::wstring CharStringEncodeWideString(const char* str);

    std::wstring CharStringEncodeWideString(const std::string& str);

    std::wstring CharStringEncodeWideString(const std::string_view& str);
}