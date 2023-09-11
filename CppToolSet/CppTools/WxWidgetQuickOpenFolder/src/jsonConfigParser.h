#pragma once

#include <string>

#include "dataDef.h"

class JsonConfigParser
{
public:
    JsonConfigParser(std::wstring path);

    std::wstring GetBasePath() const;
    std::wstring GetVsCodePath() const;
    std::wstring GetSublimeTextPath() const;
    const std::vector<BtnGroup>* GetBtnGroup() const;
    
private:
    std::wstring _basePath;
    std::wstring _vsCodePath;
    std::wstring _sublimeTextPath;
    std::vector<BtnGroup> _btnGroup;
};
