#ifndef CMAZE_H_INCLUDED
#define CMAZE_H_INCLUDED

#include"cmazeconst.h"

/***************************************************************************************
The CMediator defines an object that controls how a set of objects interact.Loose coupling
between colleague objects like CRoomManager and GUIRoomManager is achieved by having
colleagues communicate with the Mediator, rather than with each other.
CMediator act as a singleton class.
***************************************************************************************/
class CMazeEvent;
class CIManager;
class CMediator
{
public:
    ~CMediator();
    //It handles all the events those are received from CRoomManager and GUIRoomManager.
    eResult_Code HandleEvents(CMazeEvent* event);

    static CMediator* Instance();

private:
    CMediator();

    static CMediator* m_Mediator;
    CIManager* m_RoomMgr;
    CIManager* m_GuiRoomMgr;
};

#endif // CMAZE_H_INCLUDED
