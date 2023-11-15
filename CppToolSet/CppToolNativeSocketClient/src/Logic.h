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
    void UpdateIpAndPortInput();
    void UpdateInfoWindow();
    void Connect();
    void Disconnect();

private:
    std::string _ip;
    int _port;

    bool _connected = false;

    std::vector<std::string> _errorHistory;
    std::vector<std::string> _sendHistory;
    std::vector<std::string> _receiveHistory;

};