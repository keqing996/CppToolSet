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

    struct OpenFolderAppInfo
    {
        std::wstring appName;
        std::wstring appPath;
    };

private:
    OpenFolderAppInfo GetVsCodePath();
    OpenFolderAppInfo GetSublimeTextPath();
    static std::pair<wxScrolled<wxPanel>*, wxBoxSizer*> CreateMainLayout(wxWindow* topWindow);
    static wxStaticText* CreateTopTitle(wxPanel* mainPanel, wxBoxSizer* mainPanelSizer);
    static std::pair<wxPanel*, wxBoxSizer*> CreateBtnGroupLayout(wxPanel* mainPanel, wxBoxSizer* mainPanelSizer);
    static wxStaticText* CreateBtnGroupTitle(wxPanel* pTopPanel, wxBoxSizer* pTopSizer, const std::wstring& titleName);
    static std::pair<wxPanel*, wxBoxSizer*> CreateBtnListLayout(wxPanel* topPanel, wxBoxSizer* topSizer);
    static wxButton* CreateButton(wxWindow* parent, const std::wstring& label);
    static std::pair<wxPanel*, wxBoxSizer*> CreateOpenFolderPanel(wxPanel* topPanel, wxBoxSizer* topSizer);
    static wxStaticText* CreateVsCodePathDesc(wxPanel*, wxBoxSizer*, const OpenFolderAppInfo&);
    static wxCheckBox* CreateVsCodeCheckBox(wxPanel*, wxBoxSizer*);
    static wxStaticText* CreateSublimeTextPathDesc(wxPanel*, wxBoxSizer*, const OpenFolderAppInfo&);
    static wxCheckBox* CreateSublimeTextCheckBox(wxPanel*, wxBoxSizer*);
    
private:
    JsonConfigParser* _pJsonParser = nullptr;
    OpenFolderAppInfo _vsCodeInfo;
    OpenFolderAppInfo _sublimeTextInfo;

    wxPanel* _pMainPanel = nullptr;
    wxBoxSizer* _pMainPanelSizer = nullptr;

    wxPanel* _pOpenFolderPanel = nullptr;
    wxBoxSizer* _pOpenFolderPanelSizer = nullptr;
    
    wxCheckBox* _pVsCodeEnableCheckBox = nullptr;
    wxStaticText* _pVsCodeDesc = nullptr;

    wxCheckBox* _pSublimeTextEnableCheckBox = nullptr;
    wxStaticText* _pSublimeTextDesc = nullptr;
};