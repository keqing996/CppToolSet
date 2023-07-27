#pragma once

#include <string>
#include <vector>

struct Btn
{
    std::wstring name;
    std::wstring additionPath;
};

struct BtnGroup
{
    std::wstring name;
    std::vector<Btn> btnVec;
};
