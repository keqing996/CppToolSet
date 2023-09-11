#pragma once

#include <wx/wx.h>

class AppFrame : public wxFrame
{
public:
    AppFrame();

private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
};