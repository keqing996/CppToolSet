#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <ranges>
#include <algorithm>

namespace Util::String
{
    template <typename Encoding>
    using Str = std::basic_string<Encoding>;

    template <typename Encoding>
    using StrView = std::basic_string_view<Encoding>;

    template <typename EncodeFrom, typename EncodeTo>
    Str<EncodeTo> CharByCharConvert(const Str<EncodeFrom>& sourceStr)
    {
        Str<EncodeTo> toStr(sourceStr.length(), 0);

        std::transform(sourceStr.begin(), sourceStr.end(), toStr.begin(), [](EncodeFrom c)
        {
            return static_cast<EncodeTo>(c);
        });

        return toStr;
    }

    template <typename EncodeFrom, typename EncodeTo>
    Str<EncodeTo> CharByCharConvert(const StrView<EncodeFrom>& sourceStr)
    {
        Str<EncodeTo> toStr(sourceStr.length(), 0);

        std::transform(sourceStr.begin(), sourceStr.end(), toStr.begin(), [](EncodeFrom c)
        {
            return static_cast<EncodeTo>(c);
        });

        return toStr;
    }

    template <typename EncodeFrom, typename EncodeTo>
    Str<EncodeTo> CharByCharConvert(const EncodeFrom* sourceStr)
    {
        StrView<EncodeFrom> sourceStrView(sourceStr);
        Str<EncodeTo> toStr(sourceStrView.length(), 0);

        std::transform(sourceStrView.begin(), sourceStrView.end(), toStr.begin(), [](EncodeFrom c)
        {
            return static_cast<EncodeTo>(c);
        });

        return toStr;
    }

    inline
    std::string U8StringToString(const std::u8string& u8str)
    {
        return CharByCharConvert<char8_t, char>(u8str);
    }

    inline
    std::string U8StringToString(const std::u8string_view& u8str)
    {
        return CharByCharConvert<char8_t, char>(u8str);
    }

    inline
    std::string U8StringToString(const char8_t* u8str)
    {
        return CharByCharConvert<char8_t, char>(u8str);
    }

    inline
    std::u8string StringToU8String(const std::string& str)
    {
        return CharByCharConvert<char, char8_t>(str);
    }

    inline
    std::u8string StringToU8String(const std::string_view& str)
    {
        return CharByCharConvert<char, char8_t>(str);
    }

    inline
    std::u8string StringToU8String(const char* str)
    {
        return CharByCharConvert<char, char8_t>(str);
    }

    inline
    wchar_t CharToWideChar(char c)
    {
        wchar_t ret;
        mbtowc(&ret, &c, 1);
        return ret;
    }

    inline
    char WideChatToChar(wchar_t c)
    {
        char ret;
        wctomb_s(nullptr, &ret, c, 1);
        return ret;
    }

    inline
    std::wstring StringToWideString(const std::string& str)
    {
        if (str.empty())
            return {};

        const char* cstr = str.c_str();
        size_t reqsize = 0;

        int convertResult = mbstowcs_s(&reqsize, nullptr, 0, cstr, _TRUNCATE);
        if (convertResult != 0)
            return {};

        if (reqsize == 0)
            return {};

        std::vector<wchar_t> buffer(reqsize, 0);
        convertResult = mbstowcs_s(nullptr, &buffer[0], reqsize, cstr, _TRUNCATE);
        if (convertResult != 0)
            return {};

        return std::wstring(buffer.begin(), buffer.end() - 1);
    }

    inline
    std::string WideStringToString(const std::wstring& wStr)
    {
        if (wStr.empty())
            return {};

        const wchar_t* cstr = wStr.c_str();
        size_t reqsize = 0;

        int convertResult = wcstombs_s(&reqsize, nullptr, 0, cstr, _TRUNCATE);
        if (convertResult != 0)
            return {};

        if (reqsize == 0)
            return {};

        std::vector<char> buffer(reqsize, 0);
        convertResult = wcstombs_s(nullptr, &buffer[0], reqsize, cstr, _TRUNCATE);
        if (convertResult != 0)
            return {};

        return std::string(buffer.begin(), buffer.end() - 1);
    }

    template <typename Encoding, typename DelimType>
    std::vector<StrView<Encoding>> SplitView(const Str<Encoding>& inputStr, DelimType delim)
    {
        auto split = std::views::split(inputStr, delim);

        std::vector<StrView<Encoding>> result;
        for (const auto& element : split)
            result.emplace_back(element.begin(), element.end());

        return result;
    }

    template <typename Encoding, typename DelimType>
    std::vector<Str<Encoding>> Split(const Str<Encoding>& inputStr, DelimType delim)
    {
        auto split = std::views::split(inputStr, delim);

        std::vector<Str<Encoding>> result;
        for (const auto& element : split)
            result.emplace_back(element.begin(), element.end());

        return result;
    }

    template <typename Encoding>
    Str<Encoding> Join(const std::vector<Str<Encoding>>& strVec, const Str<Encoding>& delim)
    {
        std::basic_ostringstream<Encoding> oss;

        for (auto itr = strVec.begin(); itr != strVec.end(); ++itr)
        {
            oss << *itr;
            if (itr != strVec.end() - 1)
                oss << delim;
        }

        return oss.str();
    }

    template <typename Encoding>
    void Replace(Str<Encoding>& inStr, const Str<Encoding>& from, const Str<Encoding>& to)
    {
        size_t startPos = 0;
        while ((startPos = inStr.find(from, startPos)) != std::string::npos)
        {
            inStr.replace(startPos, from.length(), to);
            startPos += to.length();
        }
    }

    template <typename Encoding>
    void TrimStart(Str<Encoding>& str)
    {
        str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](Encoding ch)
        {
            return !std::isspace(static_cast<int>(ch));
        }));
    }

    template <typename Encoding>
    void TrimEnd(Str<Encoding>& str)
    {
        str.erase(std::find_if(str.rbegin(), str.rend(), [](Encoding ch)
        {
            return !std::isspace(static_cast<int>(ch));
        }).base(), str.end());
    }

    template <typename Encoding>
    void Trim(Str<Encoding>& str)
    {
        TrimStart(str);
        TrimEnd(str);
    }
}
