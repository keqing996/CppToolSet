
#include <fstream>
#include <iostream>
#include <functional>

#include "nlohmann_json/json.hpp"

int main()
{
    std::ifstream inputFile("./config/config.json");
    nlohmann::json jsonConfig;
    inputFile >> jsonConfig;

    // for each
    std::cout << "for each: \n";

    using BasicJson = nlohmann::basic_json<>;
    
    std::function<void(BasicJson&)> print = [&print](BasicJson& json) -> void
    {
        for (const auto& [key, value] : json.items())
        {
            std::cout << key << std::endl;

            if (value.is_object() || value.is_array())
                print(value);
        }
    };
    
    print(jsonConfig);
    
    // dump
    std::string str = jsonConfig.dump();

    std::cout << str;
    
    return 0;
}


