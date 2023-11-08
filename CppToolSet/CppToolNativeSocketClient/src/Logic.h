#pragma once

#include <string>
#include <vector>

namespace UiTemplate
{
    class ImGuiLogic;
};

class Logic: public UiTemplate::ImGuiLogic
{
public:
    Logic();

public:
    void Update() override;

private:
    std::string _ip;
    int _port;


};