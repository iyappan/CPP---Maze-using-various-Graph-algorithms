#ifndef CMAZECONST_H_INCLUDED
#define CMAZECONST_H_INCLUDED

#include <wx/wx.h>

typedef enum eMaze_Event
{
    EV_BUILD_ROOMS = 0,
    EV_BUILD_MAZE  = 1,
    EV_SEARCH_DEST = 2,
    EV_STOP        = 3,
    EV_CLEAR       = 4,

//GUI Events
    EV_GUI_BUILD_ROOMS = 5,
    EV_GUI_BUILD_MAZE  = 6,
    EV_GUI_SEARCH_DEST = 7,
    EV_GUI_NOTIF       = 8,
    EV_MAX,
}eMaze_Event;

typedef enum eAlg_Type
{
    ALG_DFS             = 0,
    ALG_BFS             = 1,
    ALG_DIJKISTRA       = 2,
    ALG_BELLMAN_FORD    = 3,
   //ALG_FLOYD_WARSALL   = 4,
    //ALG_FLOYD         = 5,
    //ALG_PRIMS         = 6,
    ALG_MAX,
}eAlg_Type;

enum eResult_Code
{
    RES_SUCCESS = 0,
    RES_FAILED  = 1,
    RES_MAX,
};

typedef enum eNotif
{
    MAZE_BUILD_COMPLETED = 0,
    SEARCH_COMPLETED = 1,
    DISPLAY_DATA1    = 2,
    DISPLAY_DATA2    = 3,
    DISPLAY_DATA3    = 4,
} eNotif;

//const wxColor searcCols[ALG_MAX] = {(0,0,255), (0,0,64), (255,0,255)} ;


const int DEFAULT_DEPTH  = 4;
const int DEFAULT_ROW  = 4;
const int DEFAULT_COL  = 4;
const int DEFAULT_SRC  = 0;
const int DEFAULT_DST  = 10;
const int MIN_ROW      = 10;
const int MAX_ROW      = 100;
const int MAX_COL      = 100;
/*******************************/
//EDGE_WEIGHT_RANGE must be less than VAL_INFINITE
const int VAL_INFINITE          = 100000; // some large value
const int EDGE_WEIGHT_RANGE     = 50;
/*******************************/
/*
00000000->All walls are closed
00001000->Left wall is opened    => LEFT_OPEN
00000100->Right wall is opened   => RIGHT_OPEN
00000010->Top wall is opened     => TOP_OPEN
00000001->Bottom wall is opened  => BOTTOM_OPEN
*/
const unsigned char LEFT_OPEN   = 0x01 << 3;
const unsigned char RIGHT_OPEN  = 0x01 << 2;
const unsigned char TOP_OPEN    = 0x01 << 1;
const unsigned char BOTTOM_OPEN = 0x01;

//Gui frame size for maze
const int BORDER_X1 = 300;
const int BORDER_Y1 = 10;
const int BORDER_X2 = 900;
const int BORDER_Y2 = 610;


#endif // CMAZECONST_H_INCLUDED
