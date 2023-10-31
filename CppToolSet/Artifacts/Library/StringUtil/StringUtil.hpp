#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <ranges>
#include <algorithm>

namespace StringUtil
{
    template<typename Encoding>
    using Str = std::basic_string<Encoding>;

    template<typename Encoding>
    using StrView = std::basic_string_view<Encoding>;

    class Convert
    {
    public:
        Convert() = delete;

    private:
        template<typename EncodeFrom, typename EncodeTo>
        static Str<EncodeTo> CharByCharConvert(const Str<EncodeFrom>& sourceStr)
        {
            Str<EncodeTo> toStr(sourceStr.length(), 0);

            std::transform(sourceStr.begin(), sourceStr.end(), toStr.begin(), [] (EncodeFrom c)
            {
                return (EncodeTo)c;
            });

            return toStr;
        }

        template<typename EncodeFrom, typename EncodeTo>
        static Str<EncodeTo> CharByCharConvert(const StrView<EncodeFrom>& sourceStr)
        {
            Str<EncodeTo> toStr(sourceStr.length(), 0);

            std::transform(sourceStr.begin(), sourceStr.end(), toStr.begin(), [] (EncodeFrom c)
            {
                return (EncodeTo)c;
            });

            return toStr;
        }

        template<typename EncodeFrom, typename EncodeTo>
        static Str<EncodeTo> CharByCharConvert(const EncodeFrom* sourceStr)
        {
            StrView<EncodeFrom> sourceStrView(sourceStr);
            Str<EncodeTo> toStr(sourceStrView.length(), 0);

            std::transform(sourceStrView.begin(), sourceStrView.end(), toStr.begin(), [] (EncodeFrom c)
            {
                return (EncodeTo)c;
            });

            return toStr;
        }

    public:
        inline static std::string U8StringToString(const std::u8string& u8str)
        {
            return CharByCharConvert<char8_t, char>(u8str);
        }

        inline static std::string U8StringToString(const std::u8string_view & u8str)
        {
            return CharByCharConvert<char8_t, char>(u8str);
        }

        inline static std::string U8StringToString(const char8_t* u8str)
        {
            return CharByCharConvert<char8_t, char>(u8str);
        }

        inline static std::u8string StringToU8String(const std::string& str)
        {
            return CharByCharConvert<char, char8_t>(str);
        }

        inline static std::u8string StringToU8String(const std::string_view& str)
        {
            return CharByCharConvert<char, char8_t>(str);
        }

        inline static std::u8string StringToU8String(const char* str)
        {
            return CharByCharConvert<char, char8_t>(str);
        }

        inline static wchar_t CharToWideChar(char c)
        {
            wchar_t ret;
            mbtowc(&ret, &c, 1);
            return ret;
        }

        inline static char WideChatToChar(wchar_t c)
        {
            char ret;
            wctomb_s(NULL, &ret, c, 1);	return ret;
        }

        inline static std::wstring StringToWideString(const std::string& str)
        {
            if (str.empty())
                return {};

            const char* cstr = str.c_str();
            size_t len = str.length() + 1;
            size_t reqsize = 0;
            int convertResult;

            convertResult = mbstowcs_s(&reqsize, nullptr, 0, cstr, len);
            if (convertResult != 0)
                return {};

            if (!reqsize)
                return {};

            std::vector<wchar_t> buffer(reqsize, 0);
            convertResult = mbstowcs_s(nullptr, &buffer[0], len, cstr, len);
            if (convertResult != 0)
                return {};

            return std::wstring(buffer.begin(), buffer.end() - 1);
        }
/*
        //// Convert a wide string to a narrow string//
        inline static std::string narrow(const std::wstring& str)
        {
            const wchar_t* cstr = str.c_str();
            size_t len = str.length() + 1;
            size_t reqsize = 0;
            if(wcstombs_s(&reqsize, NULL, 0, cstr, len) != 0)
                throw RecoverableException("Cannot narrow string - invalid character detected");
            if(!reqsize)
                throw RecoverableException("Failed to narrow string");
            std::vector<Byte> buffer(reqsize, 0);
            if(wcstombs_s(NULL, &buffer[0], len, cstr, len) != 0)
                throw RecoverableException("Cannot narrow string - invalid character detected");
            return std::string(buffer.begin(), buffer.end() - 1);
        }

*/

    };


    class Operation
    {
    public:
        Operation() = delete;

    public:
        template<typename Encoding, typename DelimType>
        std::vector<StrView<Encoding>> SplitView(const Str<Encoding>& inputStr, DelimType delim)
        {
            auto split = std::views::split(inputStr, delim);

            std::vector<StrView<Encoding>> result;
            for (const auto& element : split)
                result.emplace_back(element.begin(), element.end());

            return result;
        }

        template<typename Encoding, typename DelimType>
        std::vector<Str<Encoding>> Split(const Str<Encoding>& inputStr, DelimType delim)
        {
            auto split = std::views::split(inputStr, delim);

            std::vector<Str<Encoding>> result;
            for (const auto& element : split)
                result.emplace_back(element.begin(), element.end());

            return result;
        }

        template<typename Encoding>
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

        template<typename Encoding>
        void ReplaceInPlace(Str<Encoding>& inStr, const Str<Encoding>& from, const Str<Encoding>& to)
        {
            size_t startPos = 0;
            while((startPos = inStr.find(from, startPos)) != std::string::npos)
            {
                inStr.replace(startPos, from.length(), to);
                startPos += to.length();
            }
        }

        template<typename Encoding>
        Str<Encoding> Replace(const Str<Encoding>& inStr, const Str<Encoding>& from, const Str<Encoding>& to)
        {
            Str<Encoding> result = inStr;
            ReplaceInPlace(result, from, to);
            return result;
        }

        template<typename Encoding>
        void TrimInPlace(Str<Encoding>& inStr, const Str<Encoding>& trimStr)
        {
            ReplaceInPlace<Encoding>(inStr, trimStr, "");
        }

        template<typename Encoding>
        void TrimInPlace(Str<Encoding>& inStr)
        {
            TrimInPlace<Encoding>(inStr, " ");
        }

        template<typename Encoding>
        Str<Encoding> Trim(Str<Encoding>& inStr, const Str<Encoding>& trimStr)
        {
            return Replace<Encoding>(inStr, trimStr, "");
        }

        template<typename Encoding>
        Str<Encoding> Trim(Str<Encoding>& inStr)
        {
            return Trim<Encoding>(inStr, " ");
        }
    };
}


