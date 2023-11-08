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
    void Connect();

private:
    std::string _ip;
    int _port;

    std::vector<std::string> _errorHistory;
    std::vector<std::string> _sendHistory;
    std::vector<std::string> _receiveHistory;

};