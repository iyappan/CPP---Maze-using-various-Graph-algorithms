/***************************************************************
 * Name:      wxmazeMain.h
 * Purpose:   Defines Application Frame
 * Author:    Iyappan c ()
 * Created:   2015-02-03
 * Copyright: Iyappan c ()
 * License:
 **************************************************************/

#ifndef WXMAZEMAIN_H
#define WXMAZEMAIN_H

//(*Headers(wxmazeFrame)
#include <wx/stattext.h>
#include <wx/menu.h>
#include <wx/textctrl.h>
#include <wx/statbox.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/combobox.h>
#include <wx/statusbr.h>
//*)

#include<string>
using namespace std;

#include"cmazeconst.h"

class wxmazeFrame: public wxFrame
{
    public:

        wxmazeFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~wxmazeFrame();
        void DrawRoomWall(int x1, int y1, int x2, int y2, bool isToCloseWall = true);
        void DrawRoomSearchPath(int x1, int y1, int x2, int y2, eAlg_Type searchAlgType);
        void DrawBorder(bool isClean = false);
        void DrawTarget(int x1, int y1, int x2, int y2);

        void HandleSearchCompleteNotif();
        void HandleMazeBuildComplitedNotif();
        void HandleDisplayNotif(string data);

    private:
        void OnPaint(wxPaintEvent& event);

        //(*Handlers(wxmazeFrame)
        void OnQuit(wxCommandEvent& event);
       // void OnAbout(wxCommandEvent& event);
        void OnBnCloseClick(wxCommandEvent& event);
        //void OnTextCtrl2Text(wxCommandEvent& event);
        void OnBnSearchClick(wxCommandEvent& event);
        //void OnBnBuildMazeClick(wxCommandEvent& event);
        void OnBnBuildBoardClick(wxCommandEvent& event);
        void OnBnSearchClick1(wxCommandEvent& event);
        void OnBnBuildMazeClick1(wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);
        //*)

        //(*Identifiers(wxmazeFrame)
        static const long ID_STATICBOX3;
        static const long ID_STATICBOX1;
        static const long ID_STATICBOX2;
        static const long ID_STATICTEXT1;
        static const long ID_BN_CLOSE;
        static const long ID_TXT_ROWS;
        static const long ID_STATICTEXT3;
        static const long ID_BN_BUILD_MAZE;
        static const long ID_COMBOBOX1;
        static const long ID_BN_SEARCH;
        static const long ID_BN_BUILD_BOARD;
        static const long ID_STATICTEXT4;
        static const long ID_TXT_DEPTH;
        static const long ID_COMBOBOX2;
        static const long ID_TEXTCTRL1;
        static const long ID_TEXTCTRL2;
        static const long ID_STATICTEXT2;
        static const long ID_STATICTEXT5;
        static const long ID_STATICTEXT6;
        static const long ID_STATICTEXT8;
        static const long ID_STATICTEXT9;
        static const long ID_COMBOBOX3;
        static const long ID_STATICTEXT7;
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(wxmazeFrame)
        wxTextCtrl* TxtDepth;
        wxButton* BnClose;
        wxStaticBox* StaticBoxBoard;
        wxTextCtrl* TxtSource;
        wxStaticText* StDepth;
        wxStaticText* STSelectMazeAlg;
        wxComboBox* ComAnimationSpeedOption;
        wxStaticText* StRowsxCols;
        wxStaticText* StAnimaitonSeeed;
        wxStaticText* StDestination;
        wxStaticText* StPeudocode;
        wxStatusBar* StatusBar1;
        wxComboBox* ComMazeOption;
        wxButton* BnBuildBoard;
        wxStaticBox* StaticBoxSearch;
        wxStaticText* StSelectSearchalg;
        wxComboBox* ComSearchOption;
        wxTextCtrl* TxtDestinaiton;
        wxStaticBox* StaticBoxMaze;
        wxButton* BnSearch;
        wxStaticText* StSource;
        wxTextCtrl* TxtRows;
        wxButton* BnBuildMaze;
        wxStaticText* StTitle;
        //*)

        DECLARE_EVENT_TABLE()

        int m_RowXCols;

};

#endif // WXMAZEMAIN_H
