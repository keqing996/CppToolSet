#pragma once

#include <wx/wx.h>
#include "jsonConfigParser.h"

class AppFrame : public wxFrame
{
public:
    AppFrame();
    ~AppFrame() override;

private:
    void InitJson();
    void InitLayout();

    struct VsCodeAppInfo
    {
        std::wstring appName;
        std::wstring appPath;
    };

private:
    static VsCodeAppInfo GetVsCodePath();
    static std::pair<wxScrolled<wxPanel>*, wxBoxSizer*> CreateMainLayout(wxWindow* topWindow);
    static wxStaticText* CreateTopTitle(wxPanel* mainPanel, wxBoxSizer* mainPanelSizer);
    static std::pair<wxPanel*, wxBoxSizer*> CreateBtnGroupLayout(wxPanel* mainPanel, wxBoxSizer* mainPanelSizer);
    static wxStaticText* CreateBtnGroupTitle(wxPanel* pTopPanel, wxBoxSizer* pTopSizer, const std::wstring& titleName);
    static std::pair<wxPanel*, wxBoxSizer*> CreateBtnListLayout(wxPanel* topPanel, wxBoxSizer* topSizer);
    static wxButton* CreateButton(wxWindow* parent, const std::wstring& label);
    static std::pair<wxPanel*, wxBoxSizer*> CreateVsCodeInfoLayout(wxPanel* topPanel, wxBoxSizer* topSizer);
    static wxStaticText* CreateVsCodePathDesc(wxPanel* mainPanel, wxBoxSizer* mainPanelSizer, const VsCodeAppInfo& vsCodePath);
    static wxCheckBox* CreateVsCodeCheckBox(wxPanel* vsCodeInfoPanel, wxBoxSizer* vsCodeInfoPanelSizer);
    
private:
    JsonConfigParser* _pJsonParser = nullptr;
    VsCodeAppInfo _vsCodeInfo;

    wxPanel* _pVsCodePanel = nullptr;
    wxBoxSizer* _pVsCodePanelSizer = nullptr;
    wxCheckBox* _pVsCodeEnableCheckBox = nullptr;
    wxStaticText* _pVsCodeDesc = nullptr;
};