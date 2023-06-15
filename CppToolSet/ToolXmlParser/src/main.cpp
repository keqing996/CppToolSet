
#include <iostream>
#include "../tinyxml2/tinyxml2.h"

int main()
{
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError error = doc.LoadFile("./config/config.xml");
    if (error != tinyxml2::XMLError::XML_SUCCESS)
        return 1;

    const tinyxml2::XMLElement* root = doc.RootElement();
    std::cout << "root:" << root->Name() << std::endl;

    const tinyxml2::XMLElement* nodeGroupA = root->FirstChildElement("GroupA");
    if (nodeGroupA != nullptr)
    {
        std::cout << "groupA:" << nodeGroupA->Name() << std::endl;

        const tinyxml2::XMLAttribute* groupRootAttrType = nodeGroupA->FindAttribute("Type");
        if (groupRootAttrType != nullptr)
            std::cout << "groupA Type:" << groupRootAttrType->Name() << std::endl;

        const tinyxml2::XMLAttribute* groupRootAttrTest = nodeGroupA->FindAttribute("Test");
        if (groupRootAttrTest != nullptr)
            std::cout << "groupA Test:" << groupRootAttrTest->Name() << std::endl;

        const tinyxml2::XMLElement* nameNode = nodeGroupA->FirstChildElement("Name");
        while (nameNode != nullptr)
        {
            const tinyxml2::XMLElement* contentNode = nameNode->FirstChildElement("Content");
            if (contentNode != nullptr)
                std::cout << "content:" << contentNode->GetText() << std::endl;

            nameNode = nameNode->NextSiblingElement("Name");
        }
    }

    system("pause");
    return 0;
}