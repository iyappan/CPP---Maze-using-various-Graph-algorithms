#ifndef GUIEVEN_H_INCLUDED
#define GUIEVEN_H_INCLUDED
#include"cmazeconst.h"
#include"cevents.h"

class CRoom;
//These are the events used by core logic to submit on GUI for various painting.
class GUIMazeEvent : public CMazeEvent
{
public:
    virtual ~GUIMazeEvent(){}
    //Factory method to return its own kinds.
    static GUIMazeEvent* GetEvent(eMaze_Event event);

    void SetSourceRoom(CRoom *sourceRoom){m_SourceRoom = sourceRoom;}
    void SetDestRoom(CRoom *destRoom){m_DestRoom = destRoom;}
    void SetIsBackDrag(bool isBackDrag){m_IsBackDrag = isBackDrag;}
    void SetAlgType(eAlg_Type algType){m_AlgType = algType;}
    void SetMoveDirection(unsigned char moveDirection){m_MoveDirection = moveDirection;}

    CRoom *GetSourceRoom(){return m_SourceRoom;}
    CRoom *GetDestRoom(){return m_DestRoom;}
    bool GetIsBackDrag(){return m_IsBackDrag;}
    eAlg_Type GetAlgType(){return m_AlgType;}
    unsigned char GetMoveDirection(){return m_MoveDirection;}


private:
    CRoom *m_SourceRoom;
    CRoom *m_DestRoom;
    bool m_IsBackDrag;
    eAlg_Type m_AlgType;
    unsigned char m_MoveDirection;
};

/*---------------------------------------------------------------*/
class GUIBuildRoomEvent:  public GUIMazeEvent
{
public:
    GUIBuildRoomEvent(){}
    eMaze_Event GetEventID(){return EV_GUI_BUILD_ROOMS;}
};

/*---------------------------------------------------------------*/
class GUIBuildMazeEvent:  public GUIMazeEvent
{
public:
    GUIBuildMazeEvent(){}
    eMaze_Event GetEventID(){return EV_GUI_BUILD_MAZE;}
};

/*---------------------------------------------------------------*/
class GUISearchDestEvent:  public GUIMazeEvent
{
public:
    GUISearchDestEvent(){}
    eMaze_Event GetEventID(){return EV_GUI_SEARCH_DEST;}

    void SetReachedDestination(bool isReachedDest){m_IsReachedDestination = isReachedDest;}
    bool GetReachedDestination(){return m_IsReachedDestination;}

private:
    bool m_IsReachedDestination;
};
#endif // GUIEVEN_H_INCLUDED
