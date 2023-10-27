#include "../include/WinApiEncode.h"

#include <Windows.h>

namespace WinApi::Encode
{
    std::string WideStringEncodeCharString(const wchar_t* wStr, int size)
    {
        int sizeNeed = ::WideCharToMultiByte(
                CP_UTF8,
                0,
                &wStr[0],
                size,
                nullptr,
                0,
                nullptr,
                nullptr);

        std::string result (sizeNeed, 0);
        ::WideCharToMultiByte(
                CP_UTF8,
                0,
                &wStr[0],
                size,
                &result[0],
                sizeNeed,
                nullptr,
                nullptr);

        return result;
    }

    std::string WideStringEncodeCharString(const std::wstring& wStr)
    {
        if (wStr.empty())
            return {};

        return WideStringEncodeCharString(wStr.c_str(), wStr.size());
    }

    std::string WideStringEncodeCharString(const std::wstring_view& wStr)
    {
        if (wStr.empty())
            return {};

        return WideStringEncodeCharString(wStr.data(), wStr.size());
    }

    std::string WideStringEncodeCharString(const wchar_t* wStr)
    {
        if (wStr == nullptr)
            return {};

        return WideStringEncodeCharString(wStr, ::wcslen(wStr));
    }

    std::wstring CharStringEncodeWideString(const char* str, int size)
    {
        int sizeNeed = ::MultiByteToWideChar(
                CP_UTF8,
                0,
                &str[0],
                size,
                nullptr,
                0);

        std::wstring result (sizeNeed, 0);
        ::MultiByteToWideChar(
                CP_UTF8,
                0,
                &str[0],
                size,
                &result[0],
                sizeNeed);

        return result;
    }

    std::wstring CharStringEncodeWideString(const std::string& str)
    {
        if (str.empty())
            return {};

        return CharStringEncodeWideString(str.c_str(), str.size());
    }

    std::wstring CharStringEncodeWideString(const std::string_view& str)
    {
        if (str.empty())
            return {};

        return CharStringEncodeWideString(str.data(), str.size());
    }

    std::wstring CharStringEncodeWideString(const char* str)
    {
        if (str == nullptr)
            return {};

        return CharStringEncodeWideString(str, ::strlen(str));
    }
}