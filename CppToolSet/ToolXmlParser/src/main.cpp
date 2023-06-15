
#include <iostream>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp" // file
#include "rapidxml/rapidxml_print.hpp" // print

int main()
{
    rapidxml::file<> file("./config/config.xml");
    rapidxml::xml_document<> doc;
    doc.parse<0>(file.data()); // 0 means default parse flags

    const rapidxml::xml_node<>* root = doc.first_node();
    std::cout << "root:" << root->name() << std::endl;

    const rapidxml::xml_node<>* nodeGroupA = root->first_node("GroupA");
    if (nodeGroupA != nullptr)
    {
        std::cout << "groupA:" << nodeGroupA->name() << std::endl;

        const rapidxml::xml_attribute<>* groupRootAttrType = nodeGroupA->first_attribute("Type");
        if (groupRootAttrType != nullptr)
            std::cout << "groupA Type:" << groupRootAttrType->name() << std::endl;

        const rapidxml::xml_attribute<>* groupRootAttrTest = nodeGroupA->first_attribute("Test");
        if (groupRootAttrTest != nullptr)
            std::cout << "groupA Test:" << groupRootAttrTest->name() << std::endl;

        const rapidxml::xml_node<>* nameNode = nodeGroupA->first_node("Name");
        while (nameNode != nullptr)
        {
            const rapidxml::xml_node<>* contentNode = nameNode->first_node("Content");
            if (contentNode != nullptr)
                std::cout << "content:" << contentNode->value() << std::endl;

            nameNode = nameNode->next_sibling("Name");
        }
    }

    system("pause");
    return 0;
}