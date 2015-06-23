#include"cevents.h"
#include<iostream>
using namespace std;

CMazeEvent* CMazeEvent::GetEvent(eMaze_Event event)
{
    CMazeEvent* mazeEvent = 0;
    switch(event)
    {
        case EV_BUILD_ROOMS:
            mazeEvent = new CBuildRoomEvent();
        break;
        case EV_BUILD_MAZE:
            mazeEvent = new CBuildMazeEvent();
        break;
        case EV_SEARCH_DEST:
            mazeEvent = new CSearchDestEvent();
        break;
        case EV_STOP:
            mazeEvent = new CStopEvent();
        break;
        default:
            cout<<"CEventFactory::GetEvent: Invalid event = " << event <<endl;
    }
    return mazeEvent;
}

/*---------------------------------------------------------------*/
eMaze_Event CBuildRoomEvent::GetEventID()
{
    return EV_BUILD_ROOMS;
}

/*---------------------------------------------------------------*/
eMaze_Event CBuildMazeEvent::GetEventID()
{
    return EV_BUILD_MAZE;
}

/*---------------------------------------------------------------*/
eMaze_Event CSearchDestEvent::GetEventID()
{
    return EV_SEARCH_DEST;
}

/*---------------------------------------------------------------*/
eMaze_Event CStopEvent::GetEventID()
{
    return EV_STOP;
}

/*---------------------------------------------------------------*/

/*CEventFactory* CEventFactory::m_EventFactory = 0;
CEventFactory* CEventFactory::Instance()
{
    if (0 == m_EventFactory)
    {
        m_EventFactory = new CEventFactory();
    }
    return m_EventFactory;
}
*/
