#include <iomanip>
#include <iostream>
#include <Windows.h>

#include "../include/StringUtil.hpp"

void TestU8Convert()
{
    const char8_t* str = u8"中文";
    std::u8string strU8(str);
    std::u8string_view strU8View(str);

    std::string str1 = StringUtil::Convert::U8StringToString(str);
    std::string str2 = StringUtil::Convert::U8StringToString(strU8);
    std::string str3 = StringUtil::Convert::U8StringToString(strU8View);

    std::cout << str1 << "*" << std::endl;
    std::cout << str2 << "*" << std::endl;
    std::cout << str3 << "*" << std::endl;
}

void TestWideConvert()
{
    std::wstring wstr(L"中文");
    std::string str = StringUtil::Convert::WideStringToString(wstr);
    std::wstring wstr2 = StringUtil::Convert::StringToWideString(str);

    std::wcout << wstr << "*" << std::endl;
    std::cout << str << "*" << std::endl;
    std::wcout << wstr2 << "*" << std::endl;
}

void TestTrim()
{
    // this file encoded in utf8 -> char* is encoded in utf8
    const char* str = "   中文   ";
    std::string str1(str);
    std::string str2(str);
    std::string str3(str);

    StringUtil::Operation::TrimStart(str1);
    StringUtil::Operation::TrimEnd(str2);
    StringUtil::Operation::Trim(str3);

    std::cout << str << "*" << std::endl;
    std::cout << str1 << "*" << std::endl;
    std::cout << str2 << "*" << std::endl;
    std::cout << str3 << "*" << std::endl;
}

int main()
{
    ::SetConsoleOutputCP(CP_UTF8);
    std::locale::global(std::locale("zh_CN.UTF8"));

    //TestU8Convert();
    TestWideConvert();

    std::cin.get();

    return 0;
}