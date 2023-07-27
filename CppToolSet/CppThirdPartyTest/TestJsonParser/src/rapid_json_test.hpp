
#include <iostream>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"

void RapidJsonTest()
{
    std::wifstream inputFile("./config/config.json");
    std::wstringstream fileContent;
    fileContent << inputFile.rdbuf();
    inputFile.close();
    
    using DocumentW = rapidjson::GenericDocument<rapidjson::UTF16<>>;
    using ValueW = rapidjson::GenericValue<rapidjson::UTF16<>>;
    
    DocumentW doc;
    doc.Parse(fileContent.str().c_str());

    std::wcout << doc[L"tag1"].GetString() << std::endl;
    std::wcout << doc[L"tag2"].GetBool() << std::endl;

    const ValueW& arrayNode = doc[L"array1"];
    size_t arrLength = arrayNode.Size();
    for (int i = 0; i < arrLength; i++)
    {
        const ValueW& element = arrayNode[i];
        std::wcout << element[L"name"].GetString() << std::endl;
        std::wcout << element[L"desc"].GetString() << std::endl;
    }
}

void RapidJson5Test()
{
    std::wifstream inputFile("./config/config.json");
    std::wstringstream fileContent;
    fileContent << inputFile.rdbuf();
    inputFile.close();

    using DocumentW = rapidjson::GenericDocument<rapidjson::UTF16<>>;
    using ValueW = rapidjson::GenericValue<rapidjson::UTF16<>>;

    constexpr int Json5Parse = rapidjson::kParseCommentsFlag | rapidjson::kParseTrailingCommasFlag;

    DocumentW doc;
    doc.Parse<Json5Parse>(fileContent.str().c_str());

    std::wcout << doc[L"tag1"].GetString() << std::endl;
    std::wcout << doc[L"tag2"].GetBool() << std::endl;

    const ValueW& arrayNode = doc[L"array1"];
    size_t arrLength = arrayNode.Size();
    for (int i = 0; i < arrLength; i++)
    {
        const ValueW& element = arrayNode[i];
        std::wcout << element[L"name"].GetString() << std::endl;
        std::wcout << element[L"desc"].GetString() << std::endl;
    }
}