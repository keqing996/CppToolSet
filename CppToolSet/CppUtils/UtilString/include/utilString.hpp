#pragma once

#include <iterator>
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
    
}


