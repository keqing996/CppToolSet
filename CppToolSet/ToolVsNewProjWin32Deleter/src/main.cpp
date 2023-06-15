
#include <filesystem>   // cpp 17
#include <fstream>
#include <vector>
#include <iostream>

#include "rapidxml/rapidxml_ext.hpp"

using xml_node = rapidxml::xml_node<wchar_t>;
using xml_attr = rapidxml::xml_attribute<wchar_t>;

void ProcessSln(const std::filesystem::path& path)
{
    std::wifstream readStream;
    readStream.open(path, std::ios::in);

    if (!readStream.is_open())
    {
        std::wcout << path.wstring() << L" Not Open" << std::endl;
        return;
    }
    
    std::wstring oneLineContent;
    std::vector<std::wstring> resultContent;
    
    while (std::getline(readStream, oneLineContent))
    {
        if (oneLineContent.find(L"Debug|x86") != std::wstring::npos
            || oneLineContent.find(L"Release|x86") != std::wstring::npos)
        {
            std::wcout << "[Delete] " << oneLineContent << std::endl;
            continue;
        }

        resultContent.push_back(oneLineContent);
    }

    readStream.close();

    std::wofstream writeStream;
    writeStream.open(path, std::ios::out | std::ios::trunc);

    for (const auto& content : resultContent)
        writeStream << content << std::endl;

    writeStream.close();
}

void ProcessVcxprojLoopDeleteByAttr(xml_node* parentNode, const wchar_t* targetNodeName,  const wchar_t* targetAttrName)
{
    if (parentNode == nullptr)
        return;

    std::vector<xml_node*> nodeToDeleteVec;
    
    xml_node* nodeProjConfig = parentNode->first_node(targetNodeName);
    while (nodeProjConfig != nullptr)
    {
        const xml_attr* attrInclude = nodeProjConfig->first_attribute(targetAttrName);
        if (attrInclude != nullptr)
        {
            std::wstring attrValue(attrInclude->value());
            if (attrValue.find(L"Win32") != std::wstring::npos)
            {
                std::wstring content;
                rapidxml::print(std::back_inserter(content), *nodeProjConfig);
                std::wcout << "[Delete] \n" << content << std::endl;
                nodeToDeleteVec.push_back(nodeProjConfig);
            }
        }

        nodeProjConfig = nodeProjConfig->next_sibling(targetNodeName);
    }

    for (const auto nodeToDelete : nodeToDeleteVec)
        parentNode->remove_node(nodeToDelete);
}

void ProcessVcxproj(const std::filesystem::path& path)
{
    std::wfstream readStream(path);
    rapidxml::file<wchar_t> file(readStream);   // load to memory
    readStream.close();

    rapidxml::xml_document<wchar_t> doc;
    doc.parse<0>(file.data());

    xml_node* nodeProj = doc.first_node();
    if (nodeProj == nullptr)
    {
        std::cout << "Vcxproj no root" << std::endl;
        return;
    }

    xml_node* nodeItemGrp = nodeProj->first_node(L"ItemGroup");
    if (nodeItemGrp != nullptr)
        ProcessVcxprojLoopDeleteByAttr(nodeItemGrp, L"ProjectConfiguration", L"Include");

    ProcessVcxprojLoopDeleteByAttr(nodeProj, L"PropertyGroup", L"Condition");
    
    ProcessVcxprojLoopDeleteByAttr(nodeProj, L"ImportGroup", L"Condition");
    
    ProcessVcxprojLoopDeleteByAttr(nodeProj, L"ItemDefinitionGroup", L"Condition");

    // add cpp 20
    xml_node* nodeItemDefine = nodeProj->first_node(L"ItemDefinitionGroup");
    while (nodeItemDefine != nullptr)
    {
        xml_node* nodeClCompile = nodeItemDefine->first_node(L"ClCompile");
        if (nodeClCompile != nullptr)
        {
            xml_node* nodeLanguage = nodeClCompile->first_node(L"LanguageStandard");
            if (nodeLanguage != nullptr)
                nodeClCompile->remove_node(nodeLanguage);

            xml_node* nodeLanguageNew = doc.allocate_node(rapidxml::node_element, L"LanguageStandard", L"stdcpp20");
            nodeClCompile->append_node(nodeLanguageNew);
        }

        nodeItemDefine = nodeItemDefine->next_sibling(L"ItemDefinitionGroup");
    }

    // add out dir
    std::vector<xml_node*> deleteNodeVec;
    for (xml_node* nodePropertyGroup = nodeProj->first_node(L"PropertyGroup");
        nodePropertyGroup != nullptr;
        nodePropertyGroup = nodePropertyGroup->next_sibling(L"PropertyGroup"))
    {
        // only condition attr
        xml_attr* firstAttr = nodePropertyGroup->first_attribute();
        if (firstAttr == nullptr)
            continue;

        if (std::wstring(firstAttr->name()) != L"Condition")
            continue;
        
        xml_node* nodeOutDir = nodePropertyGroup->first_node(L"OutDir");
        if (nodeOutDir != nullptr)
            nodePropertyGroup->remove_node(nodeOutDir);

        xml_node* nodeIntDir = nodePropertyGroup->first_node(L"IntDir");
        if (nodeIntDir != nullptr)
            nodePropertyGroup->remove_node(nodeIntDir);

        xml_node* anyChild = nodePropertyGroup->first_node();
        if (anyChild == nullptr)
            deleteNodeVec.push_back(nodePropertyGroup);
    }

    for (auto node : deleteNodeVec)
        nodeProj->remove_node(node);

    auto AddDirProperty = [](rapidxml::xml_document<wchar_t>& doc, xml_node* nodeProj, const wchar_t* condition) -> void
    {
        xml_node* nodePropertyGroupNewDebug = doc.allocate_node(rapidxml::node_element, L"PropertyGroup");
        nodeProj->append_node(nodePropertyGroupNewDebug);

        xml_attr* attrPropertyGroupNewDebug = doc.allocate_attribute(L"Condition", condition);
        nodePropertyGroupNewDebug->append_attribute(attrPropertyGroupNewDebug);

        xml_node* nodePropertyGroupNewDebugOutDir = doc.allocate_node(rapidxml::node_element, L"OutDir", L"$(ProjectDir)\\bin\\$(Configuration)\\");
        nodePropertyGroupNewDebug->append_node(nodePropertyGroupNewDebugOutDir);

        xml_node* nodePropertyGroupNewDebugInDir = doc.allocate_node(rapidxml::node_element, L"IntDir", L"$(ProjectDir)\\intermediates\\$(Configuration)\\");
        nodePropertyGroupNewDebug->append_node(nodePropertyGroupNewDebugInDir);
    };

    AddDirProperty(doc, nodeProj, L"'$(Configuration)|$(Platform)'=='Debug|x64'");
    AddDirProperty(doc, nodeProj, L"'$(Configuration)|$(Platform)'=='Release|x64'");

    // save
    std::wofstream writeStream;
    writeStream.open(path, std::ios::out | std::ios::trunc);

    std::wstring resultStr;
    rapidxml::print(std::back_inserter(resultStr), doc);
    
    writeStream << resultStr;

    writeStream.close();
}

int main()
{
    const char* path =
#if _DEBUG
        "./testxml/";
#else
        "./";
#endif

    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (entry.status().type() == std::filesystem::file_type::directory)
            continue;

        const auto& path = entry.path();
#if _DEBUG
        if (path.extension() == ".sln.xml")
#else
        if (path.extension() == ".sln")
#endif
            ProcessSln(path);

#if _DEBUG
        if (path.extension() == ".vcxproj.xml")
#else
        if (path.extension() == ".vcxproj")
#endif
            ProcessVcxproj(path);
    }

    system("pause");

    return 0;
}
