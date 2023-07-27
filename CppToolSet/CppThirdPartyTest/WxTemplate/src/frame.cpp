#include "frame.h"
#include "../resource/acfun.xpm"

enum
{
    ID_Hello = 1
};

AppFrame::AppFrame()
    : wxFrame(nullptr, wxID_ANY, "Hello World")
{
    this->SetIcon(acfun);
    
    wxScrolled<wxPanel>* mainPanel = new wxScrolled<wxPanel>(this);
    mainPanel->SetBackgroundColour(*wxWHITE);

    wxBoxSizer* mainPanelSizer = new wxBoxSizer(wxVERTICAL);
    mainPanel->SetSizerAndFit(mainPanelSizer);

    // ---------------------------------------
    
    wxPanel* panel1 = new wxPanel(mainPanel, wxID_ANY, wxDefaultPosition, wxSize{200, 100});
    panel1->SetBackgroundColour(*wxGREEN);

    wxPanel* panel2 = new wxPanel(mainPanel, wxID_ANY, wxDefaultPosition, wxSize{200, 100});
    panel2->SetBackgroundColour(*wxBLUE);

    wxPanel* panel3 = new wxPanel(mainPanel, wxID_ANY, wxDefaultPosition, wxSize{200, 100});
    panel3->SetBackgroundColour(*wxRED);

    mainPanelSizer->Add(panel1, 0, wxEXPAND);
    mainPanelSizer->Add(panel2, 0, wxEXPAND);
    mainPanelSizer->Add(panel3, 0, wxEXPAND);

    mainPanel->SetScrollRate(10, 10);
    
    //mainPanel->SetVirtualSize(mainPanel->GetSize());
}

void AppFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void AppFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example",
        "About Hello World", wxOK | wxICON_INFORMATION);
}

void AppFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}