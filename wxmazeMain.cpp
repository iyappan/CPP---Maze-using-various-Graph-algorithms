/***************************************************************
 * Name:      wxmazeMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Iyappan c ()
 * Created:   2015-02-03
 * Copyright: Iyappan c ()
 * License:
 **************************************************************/

#include "wxmazeMain.h"
#include <wx/msgdlg.h>

#include <wx/wx.h>

//(*InternalHeaders(wxmazeFrame)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include"guiroommanager.h"
#include"croommanager.h"
#include"cmaze.h"
#include"cevents.h"
#include "windows.h"

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(wxmazeFrame)
const long wxmazeFrame::ID_STATICBOX3 = wxNewId();
const long wxmazeFrame::ID_STATICBOX1 = wxNewId();
const long wxmazeFrame::ID_STATICBOX2 = wxNewId();
const long wxmazeFrame::ID_STATICTEXT1 = wxNewId();
const long wxmazeFrame::ID_BN_CLOSE = wxNewId();
const long wxmazeFrame::ID_TXT_ROWS = wxNewId();
const long wxmazeFrame::ID_STATICTEXT3 = wxNewId();
const long wxmazeFrame::ID_BN_BUILD_MAZE = wxNewId();
const long wxmazeFrame::ID_COMBOBOX1 = wxNewId();
const long wxmazeFrame::ID_BN_SEARCH = wxNewId();
const long wxmazeFrame::ID_BN_BUILD_BOARD = wxNewId();
const long wxmazeFrame::ID_STATICTEXT4 = wxNewId();
const long wxmazeFrame::ID_TXT_DEPTH = wxNewId();
const long wxmazeFrame::ID_COMBOBOX2 = wxNewId();
const long wxmazeFrame::ID_TEXTCTRL1 = wxNewId();
const long wxmazeFrame::ID_TEXTCTRL2 = wxNewId();
const long wxmazeFrame::ID_STATICTEXT2 = wxNewId();
const long wxmazeFrame::ID_STATICTEXT5 = wxNewId();
const long wxmazeFrame::ID_STATICTEXT6 = wxNewId();
const long wxmazeFrame::ID_STATICTEXT8 = wxNewId();
const long wxmazeFrame::ID_STATICTEXT9 = wxNewId();
const long wxmazeFrame::ID_COMBOBOX3 = wxNewId();
const long wxmazeFrame::ID_STATICTEXT7 = wxNewId();
const long wxmazeFrame::idMenuQuit = wxNewId();
const long wxmazeFrame::idMenuAbout = wxNewId();
const long wxmazeFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(wxmazeFrame,wxFrame)
    //(*EventTable(wxmazeFrame)
    //*)
END_EVENT_TABLE()

//wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX | wxMINIMIZE_BOX)
wxmazeFrame::wxmazeFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(wxmazeFrame)
    wxMenuItem* MenuItem2;
    wxMenuItem* MenuItem1;
    wxMenu* Menu1;
    wxMenuBar* MenuBar1;
    wxMenu* Menu2;

    Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX | wxMINIMIZE_BOX), _T("wxID_ANY"));
    SetClientSize(wxSize(1300,702));
    Move(wxPoint(0,0));
    SetMinSize(wxSize(-1,-1));
    SetMaxSize(wxSize(-1,-1));
    SetForegroundColour(wxColour(255,255,0));
    SetBackgroundColour(wxColour(64,0,128));
    StaticBoxSearch = new wxStaticBox(this, ID_STATICBOX3, _("Search"), wxPoint(8,232), wxSize(272,168), 0, _T("ID_STATICBOX3"));
    StaticBoxBoard = new wxStaticBox(this, ID_STATICBOX1, _("Board"), wxPoint(8,64), wxSize(272,88), 0, _T("ID_STATICBOX1"));
    StaticBoxMaze = new wxStaticBox(this, ID_STATICBOX2, _("Maze"), wxPoint(8,152), wxSize(272,80), 0, _T("ID_STATICBOX2"));
    StRowsxCols = new wxStaticText(this, ID_STATICTEXT1, _("ROWS x COLS    :"), wxPoint(16,128), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    BnClose = new wxButton(this, ID_BN_CLOSE, _("Close"), wxPoint(112,456), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BN_CLOSE"));
    BnClose->SetForegroundColour(wxColour(64,0,128));
    BnClose->SetBackgroundColour(wxColour(255,128,128));
    TxtRows = new wxTextCtrl(this, ID_TXT_ROWS, _("20"), wxPoint(112,120), wxSize(32,24), 0, wxDefaultValidator, _T("ID_TXT_ROWS"));
    StTitle = new wxStaticText(this, ID_STATICTEXT3, _("Maze Using Graph Algorithms"), wxPoint(920,8), wxSize(216,32), 0, _T("ID_STATICTEXT3"));
    wxFont StTitleFont(0,wxTELETYPE,wxFONTSTYLE_NORMAL,wxBOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
    StTitle->SetFont(StTitleFont);
    BnBuildMaze = new wxButton(this, ID_BN_BUILD_MAZE, _("Build Maze"), wxPoint(200,200), wxSize(72,24), 0, wxDefaultValidator, _T("ID_BN_BUILD_MAZE"));
    BnBuildMaze->Disable();
    BnBuildMaze->SetForegroundColour(wxColour(64,0,128));
    BnBuildMaze->SetBackgroundColour(wxColour(255,128,128));
    ComMazeOption = new wxComboBox(this, ID_COMBOBOX1, wxEmptyString, wxPoint(152,168), wxSize(120,21), 0, 0, 0, wxDefaultValidator, _T("ID_COMBOBOX1"));
    ComMazeOption->SetSelection( ComMazeOption->Append(_("Depth First Search")) );
    ComMazeOption->Append(_("Breath First Search"));
    BnSearch = new wxButton(this, ID_BN_SEARCH, _("Search Paths"), wxPoint(200,368), wxSize(72,23), 0, wxDefaultValidator, _T("ID_BN_SEARCH"));
    BnSearch->Disable();
    BnSearch->SetForegroundColour(wxColour(64,0,128));
    BnSearch->SetBackgroundColour(wxColour(255,128,128));
    BnBuildBoard = new wxButton(this, ID_BN_BUILD_BOARD, _("Build Board"), wxPoint(200,120), wxSize(72,24), 0, wxDefaultValidator, _T("ID_BN_BUILD_BOARD"));
    BnBuildBoard->SetForegroundColour(wxColour(64,0,128));
    BnBuildBoard->SetBackgroundColour(wxColour(255,128,128));
    StDepth = new wxStaticText(this, ID_STATICTEXT4, _("Depth                 :"), wxPoint(16,80), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    TxtDepth = new wxTextCtrl(this, ID_TXT_DEPTH, _("4"), wxPoint(112,80), wxSize(32,21), 0, wxDefaultValidator, _T("ID_TXT_DEPTH"));
    ComSearchOption = new wxComboBox(this, ID_COMBOBOX2, wxEmptyString, wxPoint(152,248), wxSize(120,21), 0, 0, 0, wxDefaultValidator, _T("ID_COMBOBOX2"));
    ComSearchOption->SetSelection( ComSearchOption->Append(_("Depth First Search")) );
    ComSearchOption->Append(_("Breath First Search"));
    ComSearchOption->Append(_("Dijkstra Search"));
    ComSearchOption->Append(_("BellmanFord Search"));
    TxtSource = new wxTextCtrl(this, ID_TEXTCTRL1, _("0"), wxPoint(152,296), wxSize(40,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    TxtDestinaiton = new wxTextCtrl(this, ID_TEXTCTRL2, _("35"), wxPoint(152,328), wxSize(40,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
    StSource = new wxStaticText(this, ID_STATICTEXT2, _("Source                  :"), wxPoint(16,296), wxSize(96,24), 0, _T("ID_STATICTEXT2"));
    StDestination = new wxStaticText(this, ID_STATICTEXT5, _("Destination           :"), wxPoint(16,336), wxSize(120,21), 0, _T("ID_STATICTEXT5"));
    StSelectSearchalg = new wxStaticText(this, ID_STATICTEXT6, _("Select Search Alg :"), wxPoint(16,256), wxSize(112,24), 0, _T("ID_STATICTEXT6"));
    StPeudocode = new wxStaticText(this, ID_STATICTEXT8, wxEmptyString, wxPoint(928,48), wxSize(360,384), 0, _T("ID_STATICTEXT8"));
    StAnimaitonSeeed = new wxStaticText(this, ID_STATICTEXT9, _("Search Animation Speed :"), wxPoint(16,376), wxDefaultSize, 0, _T("ID_STATICTEXT9"));
    ComAnimationSpeedOption = new wxComboBox(this, ID_COMBOBOX3, wxEmptyString, wxPoint(152,368), wxSize(40,21), 0, 0, 0, wxDefaultValidator, _T("ID_COMBOBOX3"));
    ComAnimationSpeedOption->Append(_("0"));
    ComAnimationSpeedOption->SetSelection( ComAnimationSpeedOption->Append(_("1")) );
    ComAnimationSpeedOption->Append(_("2"));
    ComAnimationSpeedOption->Append(_("3"));
    ComAnimationSpeedOption->Append(_("4"));
    ComAnimationSpeedOption->Append(_("5"));
    STSelectMazeAlg = new wxStaticText(this, ID_STATICTEXT7, _("Select Maze Alg  :"), wxPoint(16,176), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);

    Connect(ID_BN_CLOSE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxmazeFrame::OnBnCloseClick);
    Connect(ID_BN_BUILD_MAZE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxmazeFrame::OnBnBuildMazeClick1);
    Connect(ID_BN_SEARCH,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxmazeFrame::OnBnSearchClick1);
    Connect(ID_BN_BUILD_BOARD,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxmazeFrame::OnBnBuildBoardClick);
  // Connect(ID_TEXTCTRL2,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&wxmazeFrame::OnTextCtrl2Text1);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxmazeFrame::OnQuit);
    Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&wxmazeFrame::OnClose);
    //*)
    this->Connect(wxEVT_PAINT, wxPaintEventHandler(wxmazeFrame::OnPaint));



    GUIRoomManager::Instance()->SetWxMazeFrame(this);
}

wxmazeFrame::~wxmazeFrame()
{
    //(*Destroy(wxmazeFrame)
    //*)
}

/*---------------------------------------------------------------*/
void wxmazeFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

/*---------------------------------------------------------------*/
/*void wxmazeFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}
*/
/*---------------------------------------------------------------*/
void wxmazeFrame::OnBnCloseClick(wxCommandEvent& event)
{
    Destroy();
}

/*---------------------------------------------------------------*/
void wxmazeFrame::OnPaint(wxPaintEvent& event)
{
  DrawBorder();
}

/*---------------------------------------------------------------*/
void wxmazeFrame::DrawBorder(bool isClean)//wxPaintEvent& event)
{
    int x1, x2, y1, y2;
    x1 = GUIRoomManager::Instance()->GetBorderX1();
    y1 = GUIRoomManager::Instance()->GetBorderY1();
    x2 = GUIRoomManager::Instance()->GetBorderX2();
    y2 = GUIRoomManager::Instance()->GetBorderY2();

    wxClientDC dc(this);
    if (isClean)
    {
        dc.SetBrush(wxColor(64,0,128));
        dc.SetPen( wxPen( wxColor(64,0,128), 5) );
    }
    else
    {
        dc.SetBrush(wxColor(200,242,168));
        dc.SetPen( wxPen( wxColor(200,242,168), 0) );
    }

    dc.DrawRectangle(x1,y1,(x2 - x1), (y2 - y1));
}

/*---------------------------------------------------------------*/
void wxmazeFrame::DrawRoomWall(int x1, int y1, int x2, int y2, bool isToCloseWall)//wxPaintEvent& event)
{
    wxClientDC dc(this);
    if (isToCloseWall)
    {
        dc.SetPen( wxPen( wxColor(0,0,0), 3 ) );
    }
    else
    {
        dc.SetPen( wxPen( wxColor(200,242,168), 3 ) );
    }

    dc.DrawLine(x1, y1, x2, y2);
}

/*---------------------------------------------------------------*/
const wxColor searchCols[ALG_MAX] = {wxColor(128,0,64), wxColor(128,128,64), wxColor(255,0,255), wxColor(64,128,128)};//,wxColor(200,128,128)} ;
void wxmazeFrame::DrawRoomSearchPath(int x1, int y1, int x2, int y2, eAlg_Type searchAlgType)
{
    wxClientDC dc(this);
    int searchAniSpeed = ComAnimationSpeedOption->GetCurrentSelection();
    Sleep(searchAniSpeed * 5);
    dc.SetBrush(searchCols[(int)searchAlgType]);
    dc.SetPen( wxPen( searchCols[(int)searchAlgType], 2) );
    dc.DrawLine(x1, y1, x2, y2);

   // dc.SetBrush(wxColor(color,color,0));
   // dc.SetPen( wxPen( wxColor(color,color,0), 0) );
   // dc.DrawRectangle(x1 + 10,y1 + 10,(x2 - x1) -10, (y2 - y1) - 10);

}

/*---------------------------------------------------------------*/
void wxmazeFrame::DrawTarget(int x1, int y1, int x2, int y2)
{
    wxClientDC dc(this);
    int radius = (x2 - x1) / 2;
	wxBrush brush(wxColour(255,0,0));

	dc.SetPen(wxPen(wxColour(255,0,0)));
	dc.SetBrush(brush);
	dc.DrawCircle(x1 + radius, y1 + radius,(x2 - x1) / 2);
}
/*---------------------------------------------------------------*/
void wxmazeFrame::OnBnBuildBoardClick(wxCommandEvent& event)
{
    //Clear the existing maze and bring to the initial state.
    CMazeEvent *stopEvent = CMazeEvent::GetEvent(EV_STOP);
    CMediator::Instance()->HandleEvents(stopEvent);

    int rows, cols;
    rows = cols = wxAtoi(TxtRows->GetValue());
    int depth = (wxAtoi(TxtDepth->GetValue()));

    if (rows < MIN_ROW || rows > MAX_ROW)
    {
        wxMessageBox("In(Rows x Cols) Minimum can be 10 and Maximum can be 100");
        return;
    }

    m_RowXCols = (rows * cols) - 1;

    BnSearch->Disable();
    BnBuildMaze->Disable();

    DrawBorder(true);
    GUIRoomManager::Instance()->PopulateRoomSize(rows, cols);
    DrawBorder();
    CBuildRoomEvent *buildRoomEvent = static_cast<CBuildRoomEvent*>(CMazeEvent::GetEvent(EV_BUILD_ROOMS));
    buildRoomEvent->SetRows(rows);
    buildRoomEvent->SetCols(cols);
    buildRoomEvent->SetDepth(depth);
    CMediator::Instance()->HandleEvents(buildRoomEvent);

    BnBuildMaze->Enable();
}

void wxmazeFrame::OnBnSearchClick1(wxCommandEvent& event)
{
    int source, destination;
    source = wxAtoi(TxtSource->GetValue());
    destination = wxAtoi(TxtDestinaiton->GetValue());
    if ( source < 0 || destination < 0)
    {
        wxMessageBox("Source or Destination must be more than 0");
        return;
    }

    if ( source == destination)
    {
        wxMessageBox("Source and Destination can not be same");
        return;
    }

    if (m_RowXCols < source || m_RowXCols < destination)
    {
        wxString msg;
        msg << "Source or Destination can not be more than maximum rooms = ";
        msg << m_RowXCols;
        wxMessageBox(msg);
        return;
    }
    BnBuildBoard->Disable();
    BnSearch->Disable();
    CSearchDestEvent *searchDestEvent = static_cast<CSearchDestEvent*>(CMazeEvent::GetEvent(EV_SEARCH_DEST));
    int selectedAlg = ComSearchOption->GetCurrentSelection();
    searchDestEvent->SetAlgType((eAlg_Type)selectedAlg);
    searchDestEvent->SetSource(source);
    searchDestEvent->SetDestination(destination);
    GUIRoomManager::Instance()->ShowTargets(source, destination);
    CMediator::Instance()->HandleEvents(searchDestEvent);
}

void wxmazeFrame::OnBnBuildMazeClick1(wxCommandEvent& event)
{
    int depth = (wxAtoi(TxtDepth->GetValue()));
    if (depth < DEFAULT_DEPTH)
    {
         wxMessageBox("Minimum value in depth can be 4");
         return;
    }
    BnBuildBoard->Disable();
    CBuildMazeEvent *buildMazeEvent = static_cast<CBuildMazeEvent*>(CMazeEvent::GetEvent(EV_BUILD_MAZE));
    int selectedAlg = ComMazeOption->GetCurrentSelection();
    buildMazeEvent->SetAlgType((eAlg_Type)selectedAlg);
    CMediator::Instance()->HandleEvents(buildMazeEvent);

    BnBuildMaze->Disable();
}

void wxmazeFrame::HandleSearchCompleteNotif()
{
    BnSearch->Enable();
    BnBuildBoard->Enable();
}

void wxmazeFrame::HandleMazeBuildComplitedNotif()
{
    BnSearch->Enable();
    BnBuildBoard->Enable();
}

void wxmazeFrame::HandleDisplayNotif(string data)
{
    StPeudocode->SetLabelText(data);
}

void wxmazeFrame::OnClose(wxCloseEvent& event)
{
    Destroy();
}


