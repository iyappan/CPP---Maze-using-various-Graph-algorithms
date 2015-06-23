#ifndef GUIROOMMANAGER_H_INCLUDED
#define GUIROOMMANAGER_H_INCLUDED

#include"cmazeconst.h"
#include"croommanager.h"
#include<vector>

using namespace std;
class wxmazeFrame;
class GUIRoom;
class GUIMazeEvent;
class GUIRoomManager : public CIManager
{
public:
    //Handles the incoming events
    virtual eResult_Code HandleEvents(CMazeEvent* event);

    void SetWxMazeFrame(wxmazeFrame *frame);
    wxmazeFrame * GetWxMazeFrame(){return m_WxMazeFrame;}

    int GetBorderX1(){return m_BorderX1;}
    int GetBorderX2(){return m_BorderX2;}
    int GetBorderY1(){return m_BorderY1;}
    int GetBorderY2(){return m_BorderY2;}

     //Find the room size based on rows and cols
    void PopulateRoomSize(int rows, int cols);

    void ShowTargets(int source, int destination);

    static GUIRoomManager* Instance();
protected:
    //Draw the default rooms.
    virtual eResult_Code InitMazeRooms(CMazeEvent* event);
    //Open the room walls based on event
    virtual eResult_Code BuildMaze(CMazeEvent* event);
    //Draw the moves for finding the destination based on event.
    virtual eResult_Code FindDesitnation(CMazeEvent* event);

    eResult_Code HandleNotif(CMazeEvent* event);

private:
    GUIRoomManager();

    //Draw the room walls based on opened sides
    void BuildRoomWalls(CRoom *room, unsigned char sides);
    //Draw the find path based on the travel path given.
    void TravelViaOpendWall(CRoom *room, unsigned char sides, eAlg_Type searchAlgType, bool isBackDrag);

    void TravellViaOpenWallSolid(CRoom *room, unsigned char sides, eAlg_Type searchAlgType);
    void TravelViaOpendWallLine(CRoom *room, unsigned char sides, eAlg_Type searchAlgType, bool isBackDrag);


    //Tells the particular side of room is opened or not
    bool IsLeftOpen(unsigned char sides);
    bool IsRightOpen(unsigned char sides);
    bool IsTopOpen(unsigned char sides);
    bool IsBottomOpen(unsigned char sides);

    //toggle the direction. Like left to right or top to bottom and vice versa.
    unsigned char GetReverseDirection(unsigned char side);

    int GetRoomWith(){return m_RoomWidth;}
    int GetRoomHeight(){return m_RoomHeight;}


    static GUIRoomManager *m_Instance;

    wxmazeFrame* m_WxMazeFrame;

    //Borders
    int m_BorderX1, m_BorderX2, m_BorderY1, m_BorderY2;
    int m_BorderColor;
    int m_BorderWidth;

    int m_RoomWidth, m_RoomHeight;
};

/*---------------------------------------------------------------*/

#endif // GUIROOMMANAGER_H_INCLUDED
