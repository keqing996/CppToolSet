
#include <wx/wx.h>

#include "frame.h"

class MyApp : public wxApp
{
public:
    bool OnInit() override
    {
        _appFrame = new AppFrame();
        _appFrame->Show(true);
        return true;
    }

private :
    AppFrame* _appFrame = nullptr;
};

wxIMPLEMENT_APP(MyApp);