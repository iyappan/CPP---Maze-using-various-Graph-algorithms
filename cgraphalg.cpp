#include<iostream>

#include"cgraphalg.h"
#include"croommanager.h"
#include"cmaze.h"
#include"guievent.h"
#include"guiroommanager.h"
#include"cminpriorityqueue.h"

using namespace std;
CGraphAlg* CGraphAlg::GetAlg(eAlg_Type algType)
{
    switch(algType)
    {
        case ALG_DFS:
            return new CDFSAlg();
        break;
        case ALG_BFS:
            return new CBFSAlg();
        break;
        case ALG_DIJKISTRA:
            return new CDijkstraAlg();
        break;
        default:
           return new CDFSAlg();
    }
}

/*---------------------------------------------------------------*/
void CGraphAlg::SubmitDrawEvent(eMaze_Event eventType, CIManager *manager, int sourceRoomNo, int DestRoomNo, eAlg_Type algType)
{
	GUIMazeEvent *event = GUIMazeEvent::GetEvent(eventType);// Object event will be removed later by receiving handler.
	event->SetSourceRoom(manager->GetRoom(sourceRoomNo));
	event->SetDestRoom(manager->GetRoom(DestRoomNo));
	event->SetIsBackDrag(false);
	event->SetAlgType(algType);
	event->SetMoveDirection(manager->GetMoveDirection(sourceRoomNo,DestRoomNo));
	CMediator::Instance()->HandleEvents(event);
}
/*---------------------------------------------------------------*/
//Depth First Search(DFS) Algorithm.
eResult_Code CDFSAlg::BuildMaze(CIManager *manager, int sourceRoomNo)
{
    static int depth = 0;
    CRoom *room;
    room = manager->GetRoom(sourceRoomNo);
    if (room == 0)
    {
        return RES_FAILED;
    }

    room->SetVisited();
    manager->GetVisitedRoomCount()++;
    ++depth;
    if (manager->GetDepth() && depth >  manager->GetDepth())
    {
        room->RemoveNotOpenedToAdj(manager);
        depth = 0;
        return RES_SUCCESS;
    }

    list<AdjNode*>& adjList = room->GetAdjacentRoomsList();
    list<AdjNode*>::iterator itr = adjList.begin();
    int itrRoomNo;
    while (itr != adjList.end())
    {
        itrRoomNo = (*(*itr))();
        if (manager->GetRoom(itrRoomNo)->IsVisited())
        {
            if(!manager->GetRoom(itrRoomNo)->IsOpenTo(room->GetRoomNo()))
            {
                manager->GetRoom(itrRoomNo)->RemoveEdge(room->GetRoomNo());
                delete *itr;
                itr = adjList.erase(itr);
                continue;
            }
            ++itr;
            continue;
        }

        //Submit event to GUI to open side from sourceRoomNo to *itr
        SubmitDrawEvent(EV_GUI_BUILD_MAZE, manager, sourceRoomNo, itrRoomNo, manager->GetBuildMazeAlgType());
        if (RES_FAILED == BuildMaze(manager, itrRoomNo))
        {
            return RES_FAILED;
        }
        ++itr;
    }

    return RES_SUCCESS;
}

/*---------------------------------------------------------------*/
eResult_Code CDFSAlg::SearchPath(CIManager *manager, int sourceRoomNo, int DestRoomNo)
{
    if( m_IsReachedDest)
    {
        return RES_SUCCESS;
    }

    CRoom *room;
    room = manager->GetRoom(sourceRoomNo);
    if (room == 0)
    {
        return RES_FAILED;
    }

    room->SetVisited();
    list<AdjNode*>& adjList = room->GetAdjacentRoomsList();
    list<AdjNode*>::iterator itr = adjList.begin();
    int itrRoomNo;
    while (itr != adjList.end())
    {
        itrRoomNo = (*(*itr))();
        if(itrRoomNo == DestRoomNo || m_IsReachedDest)
        {
            m_IsReachedDest = true;
            SubmitDrawEvent(EV_GUI_SEARCH_DEST, manager, sourceRoomNo, itrRoomNo, ALG_DFS);
            return RES_SUCCESS;
        }

        if (manager->GetRoom(itrRoomNo)->IsVisited())
        {
            ++itr;
            continue;
        }
        //Submit event to GUI to draw search moves from sourceRoomNo to *itr
        SubmitDrawEvent(EV_GUI_SEARCH_DEST, manager, sourceRoomNo, itrRoomNo, ALG_DFS);
        if (RES_FAILED == SearchPath(manager, itrRoomNo, DestRoomNo))
        {
            return RES_FAILED;
        }
        ++itr;
    }

    return RES_SUCCESS;
}

/*---------------------------------------------------------------*/
//Breath first search algorithm(BFS)
eResult_Code CBFSAlg::BuildMaze(CIManager *manager, int sourceRoomNo)
{
    CRoom *room;
    room = manager->GetRoom(sourceRoomNo);
    if (room == 0){ return RES_FAILED;}


    queue<int> roomQueue;
    list<int>::iterator itr;
    int depth = 0;

    room->SetVisited();
    manager->GetVisitedRoomCount()++;
    roomQueue.push(sourceRoomNo);
    while(!roomQueue.empty())
    {
        sourceRoomNo = roomQueue.front();
        roomQueue.pop();
        room = manager->GetRoom(sourceRoomNo);
        if (room == 0){ return RES_FAILED;}

        ++depth;
        if ( manager->GetDepth() && depth >  manager->GetDepth())
        {
            room->RemoveNotOpenedToAdj(manager);
            depth = 0;
            continue;
        }

        list<AdjNode*>& adjList = room->GetAdjacentRoomsList();
        list<AdjNode*>::iterator itr = adjList.begin();
        int itrRoomNo;
        while (itr != adjList.end())
        {
            itrRoomNo = (*(*itr))();
            if(manager->GetRoom(itrRoomNo)->IsVisited())
            {
                if(!manager->GetRoom(itrRoomNo)->IsOpenTo(room->GetRoomNo()))
                {
                    manager->GetRoom(itrRoomNo)->RemoveEdge(room->GetRoomNo());
                    delete *itr;
                    itr = adjList.erase(itr);
                    continue;
                }
                ++itr;
                continue;
            }

            roomQueue.push(itrRoomNo);
            manager->GetRoom(itrRoomNo)->SetVisited();
            manager->GetVisitedRoomCount()++;
            //Submit event to GUI to open side from sourceRoomNo to *itr
            SubmitDrawEvent(EV_GUI_BUILD_MAZE, manager, sourceRoomNo, itrRoomNo, manager->GetBuildMazeAlgType());
            ++itr;
        }
    }
    return RES_SUCCESS;
}

/*---------------------------------------------------------------*/
eResult_Code CBFSAlg::SearchPath(CIManager *manager, int sourceRoomNo, int DestRoomNo)
{
    CRoom *room = 0;
    queue<int> roomQueue;

    room = manager->GetRoom(sourceRoomNo);
    if (room == 0) { return RES_FAILED;}
    room->SetVisited();
    roomQueue.push(sourceRoomNo);
    while(!roomQueue.empty())
    {
        sourceRoomNo = roomQueue.front();
        roomQueue.pop();
        room = manager->GetRoom(sourceRoomNo);
        if (room == 0) { return RES_FAILED;}

        list<AdjNode*>& adjList = room->GetAdjacentRoomsList();
        list<AdjNode*>::iterator itr = adjList.begin();
        int itrRoomNo;
        while (itr != adjList.end())
        {
            itrRoomNo = (*(*itr))();
            if(manager->GetRoom(itrRoomNo)->IsVisited())
            {
                ++itr;
                continue;
            }

            roomQueue.push(itrRoomNo);
            manager->GetRoom(itrRoomNo)->SetVisited();
            //Submit event to GUI to draw search moves from sourceRoomNo to *itr
            SubmitDrawEvent(EV_GUI_SEARCH_DEST, manager, sourceRoomNo, itrRoomNo, ALG_BFS);
            if (DestRoomNo == itrRoomNo)
            {
                return RES_SUCCESS;
            }
            ++itr;
        }
    }

    return RES_SUCCESS;
}

/*---------------------------------------------------------------*/
//Dijkstra shortest path algorithm
eResult_Code CDijkstraAlg::SearchPath(CIManager *manager, int sourceRoomNo, int DestRoomNo)
{
    CRoom *room = 0;
    int roomCount = manager->GetRoomCounts();
    int dist[roomCount];  //It holds the distance from every node in prevVec
    int prev[roomCount]; //
    int tmpSourceRoomNo = sourceRoomNo;

    dist[tmpSourceRoomNo]   = 0;  //First node initialized with 0
    prev[tmpSourceRoomNo]   =-1;  //Undefined

    min_priority_queue<int, int> Q;
    for (int i = 0; i < roomCount; i++)
    {
        if (i != tmpSourceRoomNo)
        {
            dist[i]   = VAL_INFINITE; //Initializing with infinite.
            prev[i] = -1; //Undefined
        }
        Q.add_with_priority(dist[i], i);
    }

    bool isDestReached = false;
    while(!Q.isEmpty()&& !isDestReached)
    {
        tmpSourceRoomNo = Q.extract_min();
        room = manager->GetRoom(tmpSourceRoomNo);
        if (NULL == room){return RES_FAILED;}

        if (VAL_INFINITE == dist[tmpSourceRoomNo])
        {
            //it says that, there is no reachable nodes.
            return RES_SUCCESS;
        }

        list<AdjNode*> &adjList = room->GetAdjacentRoomsList();
        list<AdjNode*>::iterator itr = adjList.begin();
        int itrRoomNo, weight;
        while (itr != adjList.end())
        {
            itrRoomNo = (*(*itr))();
            weight = dist[tmpSourceRoomNo] + (*itr)->GetDistance();
            if (weight < dist[itrRoomNo])
            {
                dist[itrRoomNo] = weight;
                prev[itrRoomNo] = tmpSourceRoomNo;
                Q.decrease_priority(weight, itrRoomNo);
                //SubmitDrawEvent(EV_GUI_SEARCH_DEST, manager, tmpSourceRoomNo, itrRoomNo, ALG_DIJKISTRA);
                if (itrRoomNo == DestRoomNo)
                {
                    isDestReached = true;
                    break;
                }
            }
            ++itr;
        }
    }
    int tmpDestRoomNo = DestRoomNo;
    do
    {
        //Submit event to GUI to draw search moves from sourceRoomNo to *itr
        SubmitDrawEvent(EV_GUI_SEARCH_DEST, manager, prev[tmpDestRoomNo], tmpDestRoomNo, ALG_DIJKISTRA);
        tmpDestRoomNo = prev[tmpDestRoomNo];
    }while(tmpDestRoomNo != sourceRoomNo);

    return RES_SUCCESS;
}


