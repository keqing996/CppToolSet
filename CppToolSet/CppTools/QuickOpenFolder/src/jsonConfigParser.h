#pragma once

#include <string>

#include "dataDef.h"

class JsonConfigParser
{
public:
    JsonConfigParser(std::wstring path);

    std::wstring GetBasePath() const;
    const std::vector<BtnGroup>* GetBtnGroup() const;
    
private:
    std::wstring _basePath;
    std::vector<BtnGroup> _btnGroup;
};
