#include<iostream>
#include<sstream>

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
        case ALG_BELLMAN_FORD:
            return new CBellmanFordAlg();
        break;
        /*case ALG_FLOYD_WARSALL:
            return new CFloydWarshallAlg();
        break;*/
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
string CDFSAlg::GetPseudocode()
{
    ostringstream strAlg;
    strAlg << "Depth First Search (Pseudocode):\r\n"
           << "******************************\r\n"
           << "1  procedure DFS(G,v): \r\n"
           << "2     label v as discovered\r\n"
           << "3     for all edges from v to w in G.adjacentEdges(v) do\r\n"
           << "4         if vertex w is not labeled as discovered then\r\n"
           << "5              recursively call DFS(G,w)\r\n";
    return strAlg.str();

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
string CBFSAlg::GetPseudocode()
{
    ostringstream strAlg;
    strAlg << "Breath First Search (Pseudocode):\r\n"
           << "********************************\r\n"
           << "1  procedure BFS(G,v) is \r\n"
           << "2      let Q be a queue\r\n"
           << "3      Q.enqueue(v)\r\n"
           << "4      label v as discovered\r\n"
           << "5      while Q is not empty\r\n"
           << "6          v <= Q.dequeue()\r\n"
           << "7          process(v)\r\n"
           << "8          for all edges from v to w in G.adjacentEdges(v) do\r\n"
           << "9              if w is not labeled as discovered\r\n"
           << "10                  Q.enqueue(w)\r\n"
           << "11                 label w as discovered";

    return strAlg.str();

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

/*---------------------------------------------------------------*/
string CDijkstraAlg::GetPseudocode()
{
    ostringstream strAlg;

    strAlg << "Dijkstra Search (Pseudocode):\r\n"
           << "*****************************\r\n"
           << "1  function Dijkstra(Graph, source):\r\n"
           << "2      dist[source] <= 0  // Initialization\r\n"
           << "3      for each vertex v in Graph:\r\n"
           << "4          if v != source\r\n"
           << "5              dist[v] <= infinity // Unknown distance from source to v \r\n"
           << "6              prev[v] <= undefined // Predecessor of v\r\n"
           << "7          end if\r\n"
           << "8          Q.add_with_priority(v, dist[v])\r\n"
           << "9      end for \r\n"
           << "10     while Q is not empty:   // The main loop\r\n"
           << "11         u <= Q.extract_min() // Remove and return best vertex\r\n"
           << "12         for each neighbor v of u:\r\n"
           << "13             alt = dist[u] + length(u, v) \r\n"
           << "14             if alt < dist[v]\r\n"
           << "15                 dist[v] <= alt\r\n"
           << "16                 prev[v] <= u\r\n"
           << "17                 Q.decrease_priority(v, alt)\r\n"
           << "18             end if\r\n"
           << "19         end for\r\n"
           << "20     end while\r\n"
           << "21     return dist[], prev[]";

    return strAlg.str();
}
/*---------------------------------------------------------------*/
//Bellman Ford Algorithm
eResult_Code CBellmanFordAlg::SearchPath(CIManager *manager, int sourceRoomNo, int DestRoomNo)
{
    CRoom *room = 0;
    int roomCount = manager->GetRoomCounts();
    int dist[roomCount];  //It holds the distance from every node in prevVec
    int prev[roomCount]; //
    //int tmpSourceRoomNo = sourceRoomNo;

    dist[0]   = 0;  //First node initialized with 0
    prev[0]   =-1;  //Undefined

    for (int i = 1; i < roomCount; i++)
    {
            dist[i]   = VAL_INFINITE; //Initializing with infinite.
            prev[i] = -1; //Undefined
    }
    //bool isDestReached = false;
    for (int i = 1; i < roomCount - 1; i++)
    {
        for (int j = 0; j < roomCount - 1; j++)
        {
            room = manager->GetRoom(j);
            if(room == NULL)
            {
                continue;
            }
            list<AdjNode*> &adjList = room->GetAdjacentRoomsList();
            list<AdjNode*>::iterator itr = adjList.begin();
            int itrRoomNo, weight;
            while (itr != adjList.end())
            {
                itrRoomNo = (*(*itr))();
                weight = dist[j] + (*itr)->GetDistance();
                if (weight < dist[itrRoomNo])
                {
                    dist[itrRoomNo] = weight;
                    prev[itrRoomNo] = j;
                }
                ++itr;
            }
        }
    }

    int tmpDestRoomNo = DestRoomNo;
    do
    {
        SubmitDrawEvent(EV_GUI_SEARCH_DEST, manager, prev[tmpDestRoomNo], tmpDestRoomNo, ALG_BELLMAN_FORD);
        tmpDestRoomNo = prev[tmpDestRoomNo];
    }while(tmpDestRoomNo != sourceRoomNo);

    return RES_SUCCESS;
}

/*---------------------------------------------------------------*/
string CBellmanFordAlg::GetPseudocode()
{
    ostringstream strAlg;

    strAlg << "Bellman Ford Search (Pseudocode):\r\n"
           << "*****************************\r\n"
           << "function BellmanFord(list vertices, list edges, vertex source)\r\n"
           << "//Step 1: initialize graph\r\n"
           << "for each vertex v in vertices:\r\n"
           << "    if v is source then distance[v] := 0\r\n"
           << "    else distance[v] := inf\r\n"
           << "    predecessor[v] := null\r\n\n"
           << "//Step 2: relax edges repeatedly\r\n"
           << "for i from 1 to size(vertices)-1:\r\n"
           << "    for each edge (u, v) in Graph with weight w in edges:\r\n"
           << "        if distance[u] + w < distance[v]:\r\n"
           << "            distance[v] := distance[u] + w\r\n"
           << "            predecessor[v] := u\r\n\n"
           << "//Step 3: check for negative-weight cycles\r\n"
           << "for each edge (u, v) in Graph with weight w in edges:\r\n"
           << "    if distance[u] + w < distance[v]:\r\n"
           << "        error \"Graph contains a negative-weight cycle\"\r\n"
           << "return distance[], predecessor[]\r\n";

    return strAlg.str();
}

eResult_Code CFloydWarshallAlg::SearchPath(CIManager *manager, int sourceRoomNo, int DestRoomNo)
{
    /*CRoom *room = 0;
    int roomCount = manager->GetRoomCounts();
    int dist[roomCount][roomCount];
    int prev[roomCount][roomCount];
    int tmpSourceRoomNo = sourceRoomNo;

     for (int i = 0; i < roomCount; i++)
     {
         for( int j = 0; j < roomCount; j++)
         {
            if ( i == j)
            {
                dist[i][j] = 0;
            }
            else
            {
                dist[i][j]   = manager->GetRoom(i)->GetAdjWeight(j);  //First node initialized with 0
                if (dist[i][j] != VAL_INFINITE)
                {
                    prev[i][j]   = i;
                }
                else
                {
                    prev[i][j]   = -1;
                }
            }
              //Undefined
         }
     }
     int weight = 0;
     for (int k = 1; k < roomCount; k++)
     {
         for (int i = 1; i < roomCount; i++)
         {
             for ( int j = 1; j < roomCount; j++)
             {
                weight  = manager->GetRoom(i)->GetAdjWeight(k) + manager->GetRoom(k)->GetAdjWeight(j);
                if ((i != j )&& (VAL_INFINITE > weight) && dist[i][j] > weight)
                {
                    dist[i][j] = weight;
                    prev[i][j] = prev[i][k];
                }
             }
         }
     }

    int tmpDestRoomNo = DestRoomNo;
    do
    {
        SubmitDrawEvent(EV_GUI_SEARCH_DEST, manager, prev[sourceRoomNo][tmpDestRoomNo], tmpDestRoomNo, ALG_FLOYD_WARSALL);
        tmpDestRoomNo = prev[sourceRoomNo][tmpDestRoomNo];
    }while(tmpDestRoomNo != sourceRoomNo);*/

    return RES_SUCCESS;

}

string CFloydWarshallAlg::GetPseudocode()
{
    ostringstream strAlg;

    strAlg << "Floyd Warshall Search (Pseudocode):\r\n"
           << "***********************************\r\n";

    return strAlg.str();
}


