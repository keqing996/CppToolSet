
#include <fstream>
#include <sstream>

#include "jsonConfigParser.h"
#include "rapidjson/document.h"

using DocumentW = rapidjson::GenericDocument<rapidjson::UTF16<>>;
using ValueW = rapidjson::GenericValue<rapidjson::UTF16<>>;

JsonConfigParser::JsonConfigParser(std::wstring path)
{
    std::wifstream inputFile(path);
    std::wstringstream fileContent;
    fileContent << inputFile.rdbuf();
    inputFile.close();

    DocumentW doc;
    doc.Parse(fileContent.str().c_str());

    _basePath = doc[L"BasePath"].GetString();
    _vsCodePath = doc[L"VsCodePath"].GetString();
    _sublimeTextPath = doc[L"SublimeTextPath"].GetString();
    
    for (const auto& btnGrp: doc[L"BtnGrp"].GetArray())
    {
        BtnGroup grp;
        grp.name = btnGrp[L"Name"].GetString();

        for (const auto& btn: btnGrp[L"Btn"].GetArray())
        {
            Btn singleBtn;
            singleBtn.name = btn[L"Name"].GetString();
            singleBtn.additionPath = btn[L"AddPath"].GetString();
            grp.btnVec.push_back(singleBtn);
        }

        _btnGroup.push_back(grp);
    }
}

std::wstring JsonConfigParser::GetBasePath() const
{
    return _basePath;
}

std::wstring JsonConfigParser::GetVsCodePath() const
{
    return _vsCodePath;
}

std::wstring JsonConfigParser::GetSublimeTextPath() const
{
    return _sublimeTextPath;
}

const std::vector<BtnGroup>* JsonConfigParser::GetBtnGroup() const
{
    return &_btnGroup;
}
