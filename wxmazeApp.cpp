/***************************************************************
 * Name:      wxmazeApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Iyappan c ()
 * Created:   2015-02-03
 * Copyright: Iyappan c ()
 * License:
 **************************************************************/

#include "wxmazeApp.h"

//(*AppHeaders
#include "wxmazeMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(wxmazeApp);

bool wxmazeApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	wxmazeFrame* Frame = new wxmazeFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
