#include"guievent.h"
#include<iostream>
using namespace std;
GUIMazeEvent* GUIMazeEvent::GetEvent(eMaze_Event event)
{
    GUIMazeEvent* mazeEvent = 0;
    switch(event)
    {
        case EV_GUI_BUILD_ROOMS:
            mazeEvent = new GUIBuildRoomEvent();
        break;
        case EV_GUI_BUILD_MAZE:
            mazeEvent = new GUIBuildMazeEvent();
        break;
        case EV_GUI_SEARCH_DEST:
            mazeEvent = new GUISearchDestEvent();
        break;
        default:
            cout<<"CEventFactory::GetEvent: Invalid event = " << event <<endl;
    }
    return mazeEvent;
}
