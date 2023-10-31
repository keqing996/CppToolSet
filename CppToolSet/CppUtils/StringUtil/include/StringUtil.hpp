#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <ranges>
#include <algorithm>

namespace StringUtil
{
    template<typename Encoding>
    using EncodeStr = std::basic_string<Encoding>;

    template<typename Encoding>
    using EncodeStrView = std::basic_string_view<Encoding>;

    template<typename Encoding>
    using EncodeStrVec = std::vector<EncodeStr<Encoding>>;

    class Convert
    {
    public:
        Convert() = delete;

    private:
        template<typename EncodeFrom, typename EncodeTo>
        EncodeStr<EncodeTo> CharByCharConvert(const EncodeStr<EncodeFrom>& sourceStr)
        {
            EncodeStr<EncodeTo> toStr(sourceStr.length(), 0);

            std::transform(sourceStr.begin(), sourceStr.end(), toStr.begin(), [] (EncodeFrom c)
            {
                return (EncodeTo)c;
            });

            return toStr;
        }

        template<typename EncodeFrom, typename EncodeTo>
        EncodeStr<EncodeTo> CharByCharConvert(const EncodeStrView<EncodeFrom>& sourceStr)
        {
            EncodeStr<EncodeTo> toStr(sourceStr.length(), 0);

            std::transform(sourceStr.begin(), sourceStr.end(), toStr.begin(), [] (EncodeFrom c)
            {
                return (EncodeTo)c;
            });

            return toStr;
        }

        template<typename EncodeFrom, typename EncodeTo>
        EncodeStr<EncodeTo> CharByCharConvert(const EncodeFrom* sourceStr)
        {
            EncodeStrView<EncodeFrom> sourceStrView(sourceStr);
            return CharByCharConvert<EncodeFrom, EncodeTo>(sourceStr);
        }

    public:
        inline std::string U8StringToString(const std::u8string& u8str)
        {
            return CharByCharConvert<char8_t, char>(u8str);
        }

        inline std::string U8StringToString(const std::u8string_view & u8str)
        {
            return CharByCharConvert<char8_t, char>(u8str);
        }

        inline std::string U8StringToString(const char8_t* u8str)
        {
            return CharByCharConvert<char8_t, char>(u8str);
        }

        inline std::u8string StringToU8String(const std::string& str)
        {
            return CharByCharConvert<char, char8_t>(str);
        }

        inline std::u8string StringToU8String(const std::string_view& str)
        {
            return CharByCharConvert<char, char8_t>(str);
        }

        inline std::u8string StringToU8String(const char* str)
        {
            return CharByCharConvert<char, char8_t>(str);
        }
    };


    class Operation
    {
    public:
        Operation() = delete;

    public:
        template<typename Encoding, typename DelimType>
        std::vector<EncodeStrView<Encoding>> SplitView(const EncodeStr<Encoding>& inputStr, DelimType delim)
        {
            auto split = std::views::split(inputStr, delim);

            std::vector<EncodeStrView<Encoding>> result;
            for (const auto& element : split)
                result.emplace_back(element.begin(), element.end());

            return result;
        }

        template<typename Encoding, typename DelimType>
        std::vector<EncodeStr<Encoding>> Split(const EncodeStr<Encoding>& inputStr, DelimType delim)
        {
            auto split = std::views::split(inputStr, delim);

            std::vector<EncodeStr<Encoding>> result;
            for (const auto& element : split)
                result.emplace_back(element.begin(), element.end());

            return result;
        }

        template<typename Encoding>
        EncodeStr<Encoding> Join(const std::vector<EncodeStr<Encoding>>& strVec, const EncodeStr<Encoding>& delim)
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
        void ReplaceInPlace(EncodeStr<Encoding>& inStr, const EncodeStr<Encoding>& from, const EncodeStr<Encoding>& to)
        {
            size_t startPos = 0;
            while((startPos = inStr.find(from, startPos)) != std::string::npos)
            {
                inStr.replace(startPos, from.length(), to);
                startPos += to.length();
            }
        }

        template<typename Encoding>
        EncodeStr<Encoding> Replace(const EncodeStr<Encoding>& inStr, const EncodeStr<Encoding>& from, const EncodeStr<Encoding>& to)
        {
            EncodeStr<Encoding> result = inStr;
            ReplaceInPlace(result, from, to);
            return result;
        }

        template<typename Encoding>
        void TrimInPlace(EncodeStr<Encoding>& inStr, const EncodeStr<Encoding>& trimStr)
        {
            ReplaceInPlace<Encoding>(inStr, trimStr, "");
        }

        template<typename Encoding>
        void TrimInPlace(EncodeStr<Encoding>& inStr)
        {
            TrimInPlace<Encoding>(inStr, " ");
        }

        template<typename Encoding>
        EncodeStr<Encoding> Trim(EncodeStr<Encoding>& inStr, const EncodeStr<Encoding>& trimStr)
        {
            return Replace<Encoding>(inStr, trimStr, "");
        }

        template<typename Encoding>
        EncodeStr<Encoding> Trim(EncodeStr<Encoding>& inStr)
        {
            return Trim<Encoding>(inStr, " ");
        }
    };
}


