
#include "wx/settings.h"
#include "frame.h"
#include "windowsDependent.h"
#include "../resource/acfun.xpm"

AppFrame::AppFrame()
    : wxFrame(nullptr, wxID_ANY, "Quick Open Folder Tool")
{
    int userScreenWidth = wxSystemSettings::GetMetric(wxSYS_SCREEN_X, nullptr);

    this->SetIcon(acfun);

    int width = 840;
    int minHeight = static_cast<int>(1.1 * width);
    
    wxTopLevelWindowBase::SetMinSize(wxSize{width, minHeight});

    _vsCodeInfo = GetVsCodePath();
    
    InitJson();
    InitLayout();
}

AppFrame::~AppFrame()
{
    delete _pJsonParser;
}

void AppFrame::InitJson()
{
#if _DEBUG
    _pJsonParser = new JsonConfigParser(L"./config/path.json");
#else
    _pJsonParser = new JsonConfigParser(L"./path.json");
#endif
}

void AppFrame::InitLayout()
{
    auto [pMainPanel, pMainPanelSizer] = CreateMainLayout(this);

    // Title
    pMainPanelSizer->AddSpacer(20);
    CreateTopTitle(pMainPanel, pMainPanelSizer);
    pMainPanelSizer->AddSpacer(20);

    // Btn Group
    const auto pBtnGrp = _pJsonParser->GetBtnGroup();
    const auto& basePath = _pJsonParser->GetBasePath();
    for (const auto& btnGrp : *pBtnGrp)
    {
        auto [pBtnGrpPanel, pBtnGrpSizer] = CreateBtnGroupLayout(pMainPanel, pMainPanelSizer);
        
        pBtnGrpSizer->AddSpacer(25);

        CreateBtnGroupTitle(pBtnGrpPanel, pBtnGrpSizer, btnGrp.name);
        
        pBtnGrpSizer->AddSpacer(20);
        
        // Btn Panel
        {
            auto [pBtnListPanel, pBtnListSizer] = CreateBtnListLayout(pBtnGrpPanel, pBtnGrpSizer);

            pBtnListSizer->AddStretchSpacer();
            
            const auto& btnVec = btnGrp.btnVec;
            for (const auto& btn: btnVec)
            {
                wxButton* pButton = CreateButton(pBtnListPanel, btn.name);
                const std::wstring& additionPath = btn.additionPath;
                pButton->Bind(wxEVT_BUTTON,
                    [this, basePath, additionPath](wxCommandEvent& evt)
                    {
                        const std::wstring folderToOpen = basePath + additionPath;
                        if (_pVsCodeEnableCheckBox->GetValue())
                            WindowsDependent::WinExecute(_vsCodeInfo.appName, _vsCodeInfo.appPath, folderToOpen);
                        else
                            WindowsDependent::WinExecute(L"explorer.exe", folderToOpen);
                    });
                pBtnListSizer->AddStretchSpacer();
                pBtnListSizer->Add(pButton, 1,  wxALIGN_CENTER_VERTICAL);
            }
            
            pBtnListSizer->AddStretchSpacer();
            pBtnListSizer->AddStretchSpacer();  // add 2 stretch
        }
        
        pBtnGrpSizer->AddSpacer(25);
    }
    
    // Vs Code Path
    pMainPanelSizer->AddSpacer(20);
    auto [pVsCodeInfoPanel, pVsCodeInfoPanelSizer] = CreateVsCodeInfoLayout(pMainPanel, pMainPanelSizer);
    _pVsCodePanel = pVsCodeInfoPanel;
    _pVsCodePanelSizer = pVsCodeInfoPanelSizer;
    _pVsCodeEnableCheckBox = CreateVsCodeCheckBox(_pVsCodePanel, _pVsCodePanelSizer);
    _pVsCodeDesc = CreateVsCodePathDesc(_pVsCodePanel, _pVsCodePanelSizer, _vsCodeInfo);
    _pVsCodeEnableCheckBox->Bind(wxEVT_CHECKBOX,
        [this](wxCommandEvent& event)
        {
            _pVsCodeDesc->Show(_pVsCodeEnableCheckBox->GetValue());
            Layout();
      });
    pMainPanelSizer->AddSpacer(20);

    pMainPanelSizer->AddSpacer(15);
}

std::pair<wxScrolled<wxPanel>*, wxBoxSizer*> AppFrame::CreateMainLayout(wxWindow* topWindow)
{
    auto pMainPanel = new wxScrolled<wxPanel>(topWindow, wxID_ANY, wxDefaultPosition);
    auto pSizer = new wxBoxSizer(wxVERTICAL);
    pMainPanel->SetSizerAndFit(pSizer);
    pMainPanel->SetBackgroundColour(*wxWHITE);
    pMainPanel->SetScrollRate(0, 10);

    return {pMainPanel, pSizer};
}

wxStaticText* AppFrame::CreateTopTitle(wxPanel* mainPanel, wxBoxSizer* mainPanelSizer)
{
    wxStaticText* pTitle = new wxStaticText(mainPanel, wxID_ANY, wxString("QuickOpenFolderTool"));
    pTitle->SetFont(wxFont{25, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD});
    mainPanelSizer->Add(pTitle, 0, wxALIGN_CENTER);
    return pTitle;
}

AppFrame::VsCodeAppInfo AppFrame::GetVsCodePath()
{
    std::wstring prefix{L"C:\\Users\\"};
    std::wstring userName = WindowsDependent::WinGetUserName();
    std::wstring affix{L"\\AppData\\Local\\Programs\\Microsoft VS Code\\"};
    std::wstring vsCodeAppName{L"Code.exe"};
    return {vsCodeAppName, prefix + userName + affix};
}

wxStaticText* AppFrame::CreateVsCodePathDesc(wxPanel* mainPanel, wxBoxSizer* mainPanelSizer, const VsCodeAppInfo& vsCodePath)
{
    std::wstring desc = L"Current Vs Code Path:\n" + vsCodePath.appPath + vsCodePath.appName;
    
    wxStaticText* pTextVsCodePath = new wxStaticText(mainPanel, wxID_ANY, wxString(desc));
    pTextVsCodePath->SetFont(wxFont{10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM});
    mainPanelSizer->Add(pTextVsCodePath, 0, wxALIGN_CENTER);
    return pTextVsCodePath;
}

wxCheckBox* AppFrame::CreateVsCodeCheckBox(wxPanel* vsCodeInfoPanel, wxBoxSizer* vsCodeInfoPanelSizer)
{
    wxCheckBox* checkBox = new wxCheckBox(vsCodeInfoPanel, wxID_ANY, wxString("Open with VsCode"));
    vsCodeInfoPanelSizer->Add(checkBox, 0, wxALIGN_CENTER);
    checkBox->SetValue(true);
    return checkBox;
}

std::pair<wxPanel*, wxBoxSizer*> AppFrame::CreateBtnGroupLayout(wxPanel* mainPanel, wxBoxSizer* mainPanelSizer)
{
    auto pBtnGroupPanel = new wxPanel(mainPanel, wxID_ANY, wxDefaultPosition, wxSize{100, 180});
    auto pBtnGroupPanelSizer = new wxBoxSizer(wxVERTICAL);
    pBtnGroupPanel->SetBackgroundColour(wxColour{240, 240, 240});
    pBtnGroupPanel->SetSizer(pBtnGroupPanelSizer);
    mainPanelSizer->Add(pBtnGroupPanel, 0, wxEXPAND | wxALL, 15);

    return {pBtnGroupPanel, pBtnGroupPanelSizer};
}

wxStaticText* AppFrame::CreateBtnGroupTitle(wxPanel* pTopPanel, wxBoxSizer* pTopSizer, const std::wstring& titleName)
{
    wxStaticText* pBtnGrpName = new wxStaticText(pTopPanel, wxID_ANY, wxString(titleName));
    pBtnGrpName->SetFont(wxFont{16, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD});
    pTopSizer->Add(pBtnGrpName, 0, wxALIGN_CENTER);
    return pBtnGrpName;
}

std::pair<wxPanel*, wxBoxSizer*> AppFrame::CreateBtnListLayout(wxPanel* topPanel, wxBoxSizer* topSizer)
{
    auto pBtnListPanel = new wxPanel(topPanel);
    auto pBtnListPanelSizer = new wxBoxSizer(wxHORIZONTAL);
    pBtnListPanel->SetSizer(pBtnListPanelSizer);
    topSizer->Add(pBtnListPanel, 0, wxEXPAND);
    
    return {pBtnListPanel, pBtnListPanelSizer};
}

wxButton* AppFrame::CreateButton(wxWindow* parent, const std::wstring& label)
{
    wxButton* pButton = new wxButton(parent, wxID_ANY, wxString(label), wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE);
    pButton->SetMinSize(wxSize{160, 70});
    pButton->SetMaxSize(wxSize{200, 70});
    return pButton;
}

std::pair<wxPanel*, wxBoxSizer*> AppFrame::CreateVsCodeInfoLayout(wxPanel* topPanel, wxBoxSizer* topSizer)
{
    auto pVsCodeInfoPanel = new wxPanel(topPanel);
    auto pVsCodeInfoPanelSizer = new wxBoxSizer(wxVERTICAL);
    pVsCodeInfoPanel->SetSizer(pVsCodeInfoPanelSizer);
    topSizer->Add(pVsCodeInfoPanel, 0, wxEXPAND);

    return {pVsCodeInfoPanel, pVsCodeInfoPanelSizer};
}
