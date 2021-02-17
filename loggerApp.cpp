/***************************************************************
 * Name:      loggerApp.cpp
 * Purpose:   Code for Application Class
 * Author:     ()
 * Created:   2019-02-12
 * Copyright:  ()
 * License:
 **************************************************************/

#include "loggerApp.h"




//(*AppHeaders
#include "loggerMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(loggerApp);



bool loggerApp::OnInit()
{
    #if 0
        AllocConsole();
        //freopen("conin$",  "r", stdin); //console input buffer
        freopen("conout$", "w", stdout);//console output buffer
        freopen("conout$", "w", stderr); //console error device
    #endif

    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	loggerDialog Dlg(0);
    	SetTopWindow(&Dlg);
    	Dlg.ShowModal();
    	wxsOK = false;
    }
    //*)
    return wxsOK;

}



