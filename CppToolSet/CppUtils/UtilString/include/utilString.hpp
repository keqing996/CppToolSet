#pragma once

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

#include <ranges>

namespace StringUtil
{
    template<typename Encoding, typename DelimType>
    auto SplitView(
        const std::basic_string<Encoding>& inputStr,
        DelimType delim)
    -> std::vector<std::basic_string_view<Encoding>>
    {
        auto split = std::views::split(inputStr, delim);

        std::vector<std::basic_string_view<Encoding>> result;
        for (const auto& element : split)
            result.emplace_back(element.begin(), element.end());

        return result;
    }

    template<typename Encoding, typename DelimType>
    auto Split(
        const std::basic_string<Encoding>& inputStr,
        DelimType delim)
    -> std::vector<std::basic_string<Encoding>>
    {
        auto split = std::views::split(inputStr, delim);

        std::vector<std::basic_string<Encoding>> result;
        for (const auto& element : split)
            result.emplace_back(element.begin(), element.end());

        return result;
    }

    template<typename Encoding>
    auto Join(
        const std::vector<std::basic_string<Encoding>>& strVec,
        const std::basic_string<Encoding>& delim)
    -> std::basic_string<Encoding>
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

    template<typename Encoding>
    auto ReplaceInPlace(
        std::basic_string<Encoding>& inStr,
        const std::basic_string<Encoding>& from,
        const std::basic_string<Encoding>& to)
    -> void
    {
        size_t startPos = 0;
        while((startPos = inStr.find(from, startPos)) != std::string::npos)
        {
            inStr.replace(startPos, from.length(), to);
            startPos += to.length(); 
        }
    }

    template<typename Encoding>
    auto Replace(
        const std::basic_string<Encoding>& inStr,
        const std::basic_string<Encoding>& from,
        const std::basic_string<Encoding>& to)
    -> std::basic_string<Encoding>
    {
        std::basic_string<Encoding> result = inStr;
        ReplaceInPlace(result, from, to);
        return result;
    }

    template<typename Encoding>
    auto TrimInPlace(
        std::basic_string<Encoding>& inStr,
        const std::basic_string<Encoding>& trimStr)
    -> void
    {
        ReplaceInPlace<Encoding>(inStr, trimStr, "");
    }

    template<typename Encoding>
    auto Trim(
        std::basic_string<Encoding>& inStr,
        const std::basic_string<Encoding>& trimStr)
    -> std::basic_string<Encoding>
    {
        return Replace<Encoding>(inStr, trimStr, "");
    }
    
}


