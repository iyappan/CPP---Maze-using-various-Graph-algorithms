#include"guiroommanager.h"
#include "wxmazeMain.h"
#include"cmaze.h"
#include"croommanager.h"
#include"guievent.h"
#include <wx/wx.h>
#include <math.h>
GUIRoomManager *GUIRoomManager::m_Instance = 0;

GUIRoomManager::GUIRoomManager():m_WxMazeFrame(NULL),m_BorderX1(BORDER_X1), m_BorderY1(BORDER_Y1),
                                                     m_BorderX2(BORDER_X2), m_BorderY2(BORDER_Y2),
                                                     m_BorderColor(1), m_BorderWidth(3)
{
     m_RoomWidth = 100;
     m_RoomHeight =100; //Todo:: need to update
}

/*---------------------------------------------------------------*/
GUIRoomManager* GUIRoomManager::Instance()
{
    if (!m_Instance)
    {
        m_Instance = new GUIRoomManager();
    }
    return m_Instance;
}

/*---------------------------------------------------------------*/
void GUIRoomManager::SetWxMazeFrame(wxmazeFrame *frame)
{
    m_WxMazeFrame = frame;
}

/*---------------------------------------------------------------*/
eResult_Code GUIRoomManager::HandleEvents(CMazeEvent* event)
{
    eResult_Code retCode = RES_SUCCESS;
    switch(event->GetEventID())
    {
        case EV_GUI_BUILD_ROOMS:
            retCode = InitMazeRooms(event);
        break;
        case EV_GUI_BUILD_MAZE:
            retCode = BuildMaze(event);
        break;
        case EV_GUI_SEARCH_DEST:
            retCode = FindDesitnation(event);
        break;
        default:
            cout<<"GUIRoomManager:Invalid event"<<endl;
        break;
    }
    return retCode;
}

/*---------------------------------------------------------------*/
eResult_Code GUIRoomManager::InitMazeRooms(CMazeEvent* event)
{
    //PopulateRoomSize(m_MazeProperties.m_Rows, m_MazeProperties.m_Cols);
    unsigned int roomCout = GetRoomCounts();
    for (unsigned int i = 0; i < roomCout; i++)
    {
        BuildRoomWalls(m_Rooms[i], 0x00);
    }

}

/*---------------------------------------------------------------*/
eResult_Code GUIRoomManager::BuildMaze(CMazeEvent* event)
{
    GUIMazeEvent *guiEvent = static_cast<GUIMazeEvent*>(event);
    int sourceRoomNo = guiEvent->GetSourceRoom()->GetRoomNo();
    int DestRoomNo = guiEvent->GetDestRoom()->GetRoomNo();

    BuildRoomWalls(m_Rooms[sourceRoomNo], guiEvent->GetMoveDirection());
    BuildRoomWalls(m_Rooms[DestRoomNo], GetReverseDirection(guiEvent->GetMoveDirection()));

    return RES_SUCCESS;
}

/*---------------------------------------------------------------*/
eResult_Code GUIRoomManager::FindDesitnation(CMazeEvent* event)
 {
    GUIMazeEvent *guiEvent = static_cast<GUIMazeEvent*>(event);
    int sourceRoomNo = guiEvent->GetSourceRoom()->GetRoomNo();
    int DestRoomNo = guiEvent->GetDestRoom()->GetRoomNo();
    TravelViaOpendWall(m_Rooms[sourceRoomNo],guiEvent->GetMoveDirection(), guiEvent->GetAlgType(), guiEvent->GetIsBackDrag());
    TravelViaOpendWall(m_Rooms[DestRoomNo],GetReverseDirection(guiEvent->GetMoveDirection()), guiEvent->GetAlgType(), guiEvent->GetIsBackDrag());

    return RES_SUCCESS;

 }
/*---------------------------------------------------------------*/
void GUIRoomManager::PopulateRoomSize(int rows, int cols)
{
    m_BorderX2 = BORDER_X2;
    m_BorderY2 = BORDER_Y2;

    m_RoomHeight =((m_BorderY2 - m_BorderY1) / rows);
    m_RoomWidth = ((m_BorderX2 - m_BorderX1) / cols);
    m_BorderY2  = m_BorderY1 + (m_RoomHeight * rows);
    m_BorderX2  = m_BorderX1 + (m_RoomWidth * cols);
}

/*---------------------------------------------------------------*/
void GUIRoomManager::BuildRoomWalls(CRoom *room, unsigned char sides)
{
    //1) Update the new wall's status in to the current room
    room->UpdateOpendWalls(sides);

    //Get the current room's row and col numbers
    int curRow = room->GetRowNo(); //or room->GetRoomNo() / rows;
    int curCol = room->GetColNo(); //or room->GetRoomNo() % cols;

    //2) Populate left walls coordinates and draw it.
    float x1, x2, y1, y2;
    x1 = m_BorderX1 + (curCol * m_RoomWidth);
    y1 = m_BorderY1 + (curRow * m_RoomHeight);
    x2 = x1;
    y2 = y1 + m_RoomHeight;
    if (!IsLeftOpen(room->GetOpendWalls()))
    {
        m_WxMazeFrame->DrawRoomWall(x1, y1, x2, y2);
    }
    else
    {
        m_WxMazeFrame->DrawRoomWall(x1 , y1 + 2, x2 , y2 - 2, false);
    }

    //3) Populate right walls coordinates and draw it.
    x1 = m_BorderX1 + (curCol * m_RoomWidth) + m_RoomWidth;
    y1 = m_BorderY1 + (curRow * m_RoomHeight);
    x2 = x1;
    y2 = y1 + m_RoomHeight;
    if(!IsRightOpen(room->GetOpendWalls()))
    {
        m_WxMazeFrame->DrawRoomWall(x1, y1, x2, y2);
    }
    else
    {
        m_WxMazeFrame->DrawRoomWall(x1 , y1 + 2, x2 , y2 - 2, false);
    }

    //4) Populate top walls coordinates and draw it.
    x1 = m_BorderX1 + (curCol * m_RoomWidth);
    y1 = m_BorderY1 + (curRow * m_RoomHeight);
    x2 = x1 + m_RoomWidth;
    y2 = y1;
    if(!IsTopOpen(room->GetOpendWalls()))
    {
       m_WxMazeFrame->DrawRoomWall(x1, y1, x2, y2);
    }
    else
    {
        m_WxMazeFrame->DrawRoomWall(x1 + 3 , y1, x2 - 3 , y2, false);
    }

    //5) Populate bottom walls coordinates and draw it.
    x1 = m_BorderX1 + (curCol * m_RoomWidth);
    y1 = m_BorderY1 + (curRow * m_RoomHeight) + m_RoomHeight;
    x2 = x1 + m_RoomWidth;
    y2 = y1;
    if(!IsBottomOpen(room->GetOpendWalls()))
    {
        m_WxMazeFrame->DrawRoomWall(x1, y1, x2, y2);
    }
    else
    {
        m_WxMazeFrame->DrawRoomWall(x1 + 3 , y1, x2 - 3 , y2, false);
    }

}

/*---------------------------------------------------------------*/
void GUIRoomManager::TravelViaOpendWall(CRoom *room, unsigned char sides, eAlg_Type searchAlgType, bool isBackDrag)
{
    //1) Update the new wall's status in to the current room
    room->UpdateFwdPaths(searchAlgType, sides);

    //Get the current room's row and col numbers
    int curRow = room->GetRowNo(); //or room->GetRoomNo() / rows;
    int curCol = room->GetColNo(); //or room->GetRoomNo() % cols;

    //Calculate the travel position for drawing
    int roomHorizontalPath = m_RoomWidth / (int)ALG_MAX;
    int roomVerticalPath   = m_RoomHeight / (int)ALG_MAX;

    int posNo = (int)searchAlgType + 1;

    //2) Populate left path coordinates and draw it.
    int x1, x2, y1, y2;
    x1 = m_BorderX1 + (curCol * m_RoomWidth) ;
    y1 = m_BorderY1 + (curRow * m_RoomHeight) + (roomVerticalPath * posNo);
    x2 = x1 + (roomHorizontalPath * posNo);
    y2 = y1;
    if (IsLeftOpen(room->GetFwdPaths(searchAlgType)))
    {
        m_WxMazeFrame->DrawRoomSearchPath(x1, y1, x2, y2, searchAlgType);
    }

    //3) Populate right path coordinates and draw it.
    x1 = m_BorderX1 + (curCol * m_RoomWidth) + (roomHorizontalPath * posNo);
    y1 = m_BorderY1 + (curRow * m_RoomHeight) + (roomVerticalPath * posNo);
    x2 = x1 + (roomHorizontalPath * ALG_MAX);
    y2 = y1;
    if(IsRightOpen(room->GetFwdPaths(searchAlgType)))
    {
        m_WxMazeFrame->DrawRoomSearchPath(x1, y1, x2, y2, searchAlgType);
    }

    //4) Populate top path coordinates and draw it.
    x1 = m_BorderX1 + (curCol * m_RoomWidth) + (roomHorizontalPath * posNo);
    y1 = m_BorderY1 + (curRow * m_RoomHeight);
    x2 = x1;
    y2 = y1 + (roomVerticalPath * posNo);
    if(IsTopOpen(room->GetFwdPaths(searchAlgType)))
    {
        m_WxMazeFrame->DrawRoomSearchPath(x1, y1, x2, y2, searchAlgType);
    }

    //5) Populate bottom path coordinates and draw it.
    x1 = m_BorderX1 + (curCol * m_RoomWidth) + (roomHorizontalPath * posNo);
    y1 = m_BorderY1 + (curRow * m_RoomHeight) + (roomVerticalPath * posNo);
    x2 = x1 ;
    y2 = m_BorderY1 + (curRow * m_RoomHeight) + m_RoomHeight;//y1 + (roomVerticalPath * posNo);
    if(IsBottomOpen(room->GetFwdPaths(searchAlgType)))
    {
       m_WxMazeFrame->DrawRoomSearchPath(x1, y1, x2, y2, searchAlgType);
    }
}

/*---------------------------------------------------------------*/
unsigned char GUIRoomManager::GetReverseDirection(unsigned char side)
{
    switch(side)
    {
        case LEFT_OPEN: return RIGHT_OPEN;
        case RIGHT_OPEN: return LEFT_OPEN;
        case TOP_OPEN: return BOTTOM_OPEN;
        case BOTTOM_OPEN: return TOP_OPEN;
        default:
            return 0x00;
    }
}

/*---------------------------------------------------------------*/
bool GUIRoomManager::IsLeftOpen(unsigned char sides)
{
    return (sides & LEFT_OPEN);
}

/*---------------------------------------------------------------*/
bool GUIRoomManager::IsRightOpen(unsigned char sides)
{
    return (sides & RIGHT_OPEN);
}

/*---------------------------------------------------------------*/
bool GUIRoomManager::IsTopOpen(unsigned char sides)
{
    return (sides & TOP_OPEN);
}

/*---------------------------------------------------------------*/
bool GUIRoomManager::IsBottomOpen(unsigned char sides)
{
    return (sides & BOTTOM_OPEN);
}

/*---------------------------------------------------------------*/
void GUIRoomManager::ShowTargets(int source, int destination)
{
     //Get the current room's row and col numbers
    int row = m_Rooms[source]->GetRowNo(); //or room->GetRoomNo() / rows;
    int col = m_Rooms[source]->GetColNo(); //or room->GetRoomNo() % cols;

    int x1, x2, y1, y2;
    x1 = m_BorderX1 + (col * m_RoomWidth);
    y1 = m_BorderY1 + (row * m_RoomHeight);
    x2 = x1 + m_RoomWidth;
    y2 = y1 + m_RoomHeight;
    m_WxMazeFrame->DrawTarget(x1, y1, x2, y2);

    row = m_Rooms[destination]->GetRowNo(); //or room->GetRoomNo() / rows;
    col = m_Rooms[destination]->GetColNo(); //or room->GetRoomNo() % cols;

    x1 = m_BorderX1 + (col * m_RoomWidth);
    y1 = m_BorderY1 + (row * m_RoomHeight);
    x2 = x1 + m_RoomWidth;
    y2 = y1 + m_RoomHeight;
    m_WxMazeFrame->DrawTarget(x1, y1, x2, y2);
}
/*---------------------------------------------------------------*/
