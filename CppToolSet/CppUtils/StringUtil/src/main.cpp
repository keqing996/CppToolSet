#include <iomanip>
#include <iostream>

#include "../include/StringUtil.hpp"


int main()
{
    std::string str = "testtest";
    std::wstring wstr = StringUtil::Convert::StringToWideString(str);

    auto p = wstr.data();

    auto len = wcslen(p);

    return 0;
}