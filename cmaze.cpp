#include <iostream>

#include"cmaze.h"
#include"cevents.h"
#include"croommanager.h"
#include"guiroommanager.h"

CMediator* CMediator::m_Mediator = 0;
CMediator::CMediator()
{
    m_RoomMgr    = CRoomManager::Instance();
    m_GuiRoomMgr = GUIRoomManager::Instance();
}

/*---------------------------------------------------------------*/
CMediator::~CMediator()
{
    delete m_RoomMgr;
    delete m_GuiRoomMgr;
}
/*---------------------------------------------------------------*/
CMediator* CMediator::Instance()
{
    if(0 == m_Mediator)
    {
        m_Mediator= new CMediator();
    }
    return m_Mediator;
}

/*---------------------------------------------------------------*/
eResult_Code CMediator::HandleEvents(CMazeEvent* event)
{
    eResult_Code retCode = RES_SUCCESS;
    switch(event->GetEventID())
    {
        case EV_BUILD_ROOMS:
        case EV_BUILD_MAZE:
        case EV_SEARCH_DEST:
        case EV_STOP:
            retCode = m_RoomMgr->HandleEvents(event);
        break;
        case EV_GUI_BUILD_ROOMS:
        case EV_GUI_BUILD_MAZE:
        case EV_GUI_SEARCH_DEST:
            retCode = m_GuiRoomMgr->HandleEvents(event);
        default:
            cout<<"CMediator:Invalid event"<<endl;
        break;
        delete event;
    }
    return retCode;
}

/*---------------------------------------------------------------*/

