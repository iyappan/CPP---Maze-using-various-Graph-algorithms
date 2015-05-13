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
const long wxmazeFrame::idMenuQuit = wxNewId();
const long wxmazeFrame::idMenuAbout = wxNewId();
const long wxmazeFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(wxmazeFrame,wxFrame)
    //(*EventTable(wxmazeFrame)
    //*)
END_EVENT_TABLE()

wxmazeFrame::wxmazeFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(wxmazeFrame)
    wxMenuItem* MenuItem2;
    wxMenuItem* MenuItem1;
    wxMenu* Menu1;
    wxMenuBar* MenuBar1;
    wxMenu* Menu2;

    Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(1043,702));
    Move(wxPoint(0,0));
    SetMinSize(wxSize(-1,-1));
    SetMaxSize(wxSize(-1,-1));
    SetBackgroundColour(wxColour(0,0,255));
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("ROWS x COLS"), wxPoint(16,128), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    BnClose = new wxButton(this, ID_BN_CLOSE, _("Close"), wxPoint(112,392), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BN_CLOSE"));
    TxtRows = new wxTextCtrl(this, ID_TXT_ROWS, _("20"), wxPoint(112,120), wxSize(32,24), 0, wxDefaultValidator, _T("ID_TXT_ROWS"));
    StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Maze Using Graph Algorithms"), wxPoint(64,8), wxSize(216,32), 0, _T("ID_STATICTEXT3"));
    BnBuildMaze = new wxButton(this, ID_BN_BUILD_MAZE, _("Build Maze"), wxPoint(200,184), wxSize(72,24), 0, wxDefaultValidator, _T("ID_BN_BUILD_MAZE"));
    BnBuildMaze->Disable();
    ComboBox1 = new wxComboBox(this, ID_COMBOBOX1, wxEmptyString, wxPoint(16,184), wxSize(152,21), 0, 0, 0, wxDefaultValidator, _T("ID_COMBOBOX1"));
    ComboBox1->SetSelection( ComboBox1->Append(_("Depth First Search")) );
    ComboBox1->Append(_("Breath First Search"));
    BnSearch = new wxButton(this, ID_BN_SEARCH, _("Search Paths"), wxPoint(192,328), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BN_SEARCH"));
    BnSearch->Disable();
    BnBuildBoard = new wxButton(this, ID_BN_BUILD_BOARD, _("Build Board"), wxPoint(200,120), wxSize(72,24), 0, wxDefaultValidator, _T("ID_BN_BUILD_BOARD"));
    StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Depth"), wxPoint(16,80), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    TxtDepth = new wxTextCtrl(this, ID_TXT_DEPTH, _("4"), wxPoint(112,80), wxSize(32,21), 0, wxDefaultValidator, _T("ID_TXT_DEPTH"));
    ComboBox2 = new wxComboBox(this, ID_COMBOBOX2, wxEmptyString, wxPoint(144,248), wxSize(128,21), 0, 0, 0, wxDefaultValidator, _T("ID_COMBOBOX2"));
    ComboBox2->SetSelection( ComboBox2->Append(_("Depth First Search")) );
    ComboBox2->Append(_("Breath First Search"));
    ComboBox2->Append(_("Dijkstra Search"));
    TxtSource = new wxTextCtrl(this, ID_TEXTCTRL1, _("0"), wxPoint(144,296), wxSize(40,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    TxtDestinaiton = new wxTextCtrl(this, ID_TEXTCTRL2, _("35"), wxPoint(144,328), wxSize(40,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Source"), wxPoint(16,296), wxSize(56,24), 0, _T("ID_STATICTEXT2"));
    StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Destination"), wxPoint(16,336), wxSize(64,21), 0, _T("ID_STATICTEXT5"));
    StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("Select Search Alg"), wxPoint(16,256), wxSize(112,24), 0, _T("ID_STATICTEXT6"));
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
    Connect(ID_TEXTCTRL2,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&wxmazeFrame::OnTextCtrl2Text1);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxmazeFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxmazeFrame::OnAbout);
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
void wxmazeFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

/*---------------------------------------------------------------*/
void wxmazeFrame::OnBnCloseClick(wxCommandEvent& event)
{
    Close();
}

/*---------------------------------------------------------------*/
void wxmazeFrame::OnTextCtrl2Text(wxCommandEvent& event)
{
   // GUIRoomManager::Instance()->GUIMoveForBuildMaze(NULL);
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
        dc.SetBrush(*wxBLUE_BRUSH);
        dc.SetPen( wxPen( wxColor(0,0,255), 5) );
    }
    else
    {
        dc.SetBrush(*wxWHITE_BRUSH);
        dc.SetPen( wxPen( wxColor(255,255,255), 0) );
    }


    dc.DrawRectangle(x1,y1,(x2 - x1), (y2 - y1));
}

/*---------------------------------------------------------------*/
void wxmazeFrame::DrawRoomWall(int x1, int y1, int x2, int y2, bool isToCloseWall)//wxPaintEvent& event)
{
    wxClientDC dc(this);
    if (isToCloseWall)
    {
        dc.SetPen( wxPen( wxColor(0,255,0), 3 ) );
    }
    else
    {
        dc.SetPen( wxPen( wxColor(255,255,255), 3 ) );
    }


    dc.DrawLine(x1, y1, x2, y2);

}

/*---------------------------------------------------------------*/
void wxmazeFrame::DrawRoomSearchPath(int x1, int y1, int x2, int y2, eAlg_Type searchAlgType)
{
    wxClientDC dc(this);
    Sleep(1);
    int color = 100 * ((int)searchAlgType + 2);
    dc.SetPen( wxPen( wxColor(color,color,0), 1) );
    dc.DrawLine(x1, y1, x2, y2);
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
    CSearchDestEvent *searchDestEvent = static_cast<CSearchDestEvent*>(CMazeEvent::GetEvent(EV_SEARCH_DEST));
    int selectedAlg = ComboBox2->GetCurrentSelection();
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
    CBuildMazeEvent *buildMazeEvent = static_cast<CBuildMazeEvent*>(CMazeEvent::GetEvent(EV_BUILD_MAZE));
    int selectedAlg = ComboBox1->GetCurrentSelection();
    buildMazeEvent->SetAlgType((eAlg_Type)selectedAlg);
    CMediator::Instance()->HandleEvents(buildMazeEvent);

    BnBuildMaze->Disable();
    BnSearch->Enable();
}

void wxmazeFrame::OnTextCtrl2Text1(wxCommandEvent& event)
{
}
