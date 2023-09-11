
#include <wx/wx.h>

#include "frame.h"

class MyApp : public wxApp
{
public:
    bool OnInit() override
    {
        _pAppFrame = new AppFrame();
        _pAppFrame->Show(true);
        return true;
    }

private :
    AppFrame* _pAppFrame = nullptr;
};

wxIMPLEMENT_APP(MyApp);