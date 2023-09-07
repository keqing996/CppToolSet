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

    
    /*
    template<typename Encoding>
    auto Split(
        const std::basic_string<Encoding>& inputStr,
        Encoding delim
        )
    -> std::vector<std::basic_string<Encoding>>
    {
        using EncodingString = std::basic_string<Encoding>;
        using EncodingStringStream = std::basic_istringstream<Encoding>;
        
        std::vector<EncodingString> result;
        auto bi = std::back_inserter(result);
        
        EncodingStringStream iss(inputStr);
        EncodingString item;
        
        while (std::getline(iss, item, delim))
            *bi++ = item;

        return result;
    }

    template<typename Encoding>
    auto Split(
        const std::basic_string<Encoding>& inputStr,
        const std::basic_string<Encoding>& delim
        )
    -> std::vector<std::basic_string<Encoding>>
    {
        using EncodingString = std::basic_string<Encoding>;
        using EncodingStringStream = std::basic_istringstream<Encoding>;
        
        std::vector<EncodingString> result;
        auto bi = std::back_inserter(result);
        
        EncodingStringStream iss(inputStr);
        EncodingString item;
        
        while (std::getline(iss, item, delim))
        {
            if (!item.empty())
                *bi++ = item;
        }

        return result;
    }

    template<class Iter, typename Encoding>
    auto Join(
        const Iter& itrBegin,
        const Iter& itrEnd,
        const std::basic_string<Encoding>& delim)
    -> std::basic_string<Encoding>
    {
        using EncodingString = std::basic_string<Encoding>;
        using EncodingStringStream = std::basic_ostringstream<Encoding>;
        using EncodingStringStreamItr = std::ostream_iterator<EncodingString>;
        
        EncodingStringStream oss;
        std::copy(itrBegin, itrEnd, EncodingStringStreamItr(oss, delim.c_str()));

        return oss.str();
    }
    */
}


