#include<iostream>
#include"croommanager.h"
#include"cgraphalg.h"
#include <cstdlib>
#include <time.h>
#include"cevents.h"
#include"guievent.h"
#include"cmaze.h"
#include"cmazethread.h"

SMazeProperties::SMazeProperties():m_Rows(DEFAULT_ROW), m_Cols(DEFAULT_COL), m_MazeAlgType(ALG_DFS),
                                    m_Source(-1), m_Destination(-1),m_VisitedRoomCount(0),m_Depth(DEFAULT_DEPTH)
{

}

/*---------------------------------------------------------------*/
SMazeProperties CIManager::m_MazeProperties;
vector<CRoom*> CIManager::m_Rooms;
CIManager::CIManager()
{
}

/*---------------------------------------------------------------*/
CIManager::~CIManager()
{
    ClearRooms();
}

/*---------------------------------------------------------------*/
SMazeProperties& CIManager::GetMazeProperties()
{
    return m_MazeProperties;
}

/*---------------------------------------------------------------*/
vector<CRoom*>& CIManager::GetRooms()
{
    return m_Rooms;
}

/*---------------------------------------------------------------*/
void CIManager::ClearRooms()
{
    for (unsigned int i = 0; i < m_Rooms.size(); i++)
    {
        delete m_Rooms[i];
    }
    m_Rooms.clear();
}

/*---------------------------------------------------------------*/
void CIManager::ClearVisitedStatus()
{
    for (unsigned int i = 0; i < m_Rooms.size(); i++)
    {
        m_Rooms[i]->SetVisited(false);
    }
}

/*---------------------------------------------------------------*/
unsigned char CIManager::GetMoveDirection(int srcRoomNo, int DestRoomNo)
{
    //Top
    if ( DestRoomNo == (srcRoomNo - m_MazeProperties.m_Cols))
    {
        return TOP_OPEN;
    }
    //down
    if (DestRoomNo == (srcRoomNo + m_MazeProperties.m_Cols))
    {
        return BOTTOM_OPEN;
    }
    //Left
    if ( (srcRoomNo % m_MazeProperties.m_Cols) > 0)
    {
        if (DestRoomNo == (srcRoomNo - 1))
        {
            return LEFT_OPEN;
        }
    }
    //right
    if ( (srcRoomNo % m_MazeProperties.m_Cols) < (m_MazeProperties.m_Cols -1) )
    {
        if (DestRoomNo == srcRoomNo + 1)
        {
            return RIGHT_OPEN;
        }
    }
}

/*---------------------------------------------------------------*/
CRoom* CIManager::GetRoom(int roomNo)
{
    if (m_Rooms.size() < roomNo)
    {
        cout<<"CRoomManager::GetRoom : Wrong room no ="<< roomNo<<" Total rooms = "<<m_Rooms.size()<<endl;
        return 0;
    }
    return m_Rooms[roomNo];
}

/*---------------------------------------------------------------*/
void CIManager::PrintMaze()
{
   /* list<int>::iterator itr;
    for (unsigned int i = 0; i < m_Rooms.size(); i++)
    {
        cout<<"\nFrom Room No = " << i;
        list<int>& adjList = m_Rooms[i]->GetAdjacentRoomsList();
        itr = adjList.begin();
        while (itr != adjList.end())
        {
            cout<<"==>"<<*itr;
            ++itr;
        }
    }*/
}

/*---------------------------------------------------------------*/
int CIManager::GetRoomCounts()
{
    return m_Rooms.size();
}

/*---------------------------------------------------------------*/
void CIManager::FinalizeDestinationRoom(int roomNo)
{

    cout<<"Room count = "<< GetRoomCounts()<<" visited count = "<< m_MazeProperties.m_VisitedRoomCount << " Room no ="<<roomNo<<endl;
    if (GetRoomCounts() == m_MazeProperties.m_VisitedRoomCount)
    {
        m_MazeProperties.m_Destination = roomNo;
        cout<<"Destination is = "<< m_MazeProperties.m_Destination<<endl;
    }
}
/*---------------------------------------------------------------*/
CRoomManager* CRoomManager::m_Instance = 0;
CRoomManager::CRoomManager()
{
    ClearRooms();
}

/*---------------------------------------------------------------*/
CRoomManager* CRoomManager::Instance()
{
    if (!m_Instance)
    {
        m_Instance = new CRoomManager();
    }
    return m_Instance;
}

/*---------------------------------------------------------------*/
eResult_Code CRoomManager::HandleEvents(CMazeEvent* event)
{
    eResult_Code retCode = RES_SUCCESS;
    switch(event->GetEventID())
    {
        case EV_BUILD_ROOMS:
            retCode = InitMazeRooms(event);
        break;
        case EV_BUILD_MAZE:
            retCode = BuildMaze(event);
        break;
        case EV_SEARCH_DEST:
            retCode = FindDesitnation(event);
        break;
        case EV_STOP:
            ClearRooms();
        break;
        default:
            cout<<"CRoomManager:Invalid event"<<endl;
        break;
    }
    return retCode;
}

/*---------------------------------------------------------------*/
eResult_Code CRoomManager::InitMazeRooms(CMazeEvent* event)
{
    //Stores all the received properties into the maze property member.
    CBuildRoomEvent* buildRoomEvent = static_cast<CBuildRoomEvent*>(event);
    m_MazeProperties.m_Rows         = buildRoomEvent->GetRows();
    m_MazeProperties.m_Cols         = buildRoomEvent->GetCols();
    m_MazeProperties.m_Depth        = buildRoomEvent->GetDepth();

    //Clear if any existing maze graph.
    ClearRooms();
    //Create the rooms based on rows and cols.
    BuildRooms();
    //Form every room, find the adjacent and populate the graph.
    BuildDefaultGraph();
    cout<<"----------------Default Graph------------------"<<endl;
    PrintMaze();

    //Update the GUI to generate the default rooms.
    GUIMazeEvent *guiEvent = GUIMazeEvent::GetEvent(EV_GUI_BUILD_ROOMS);
    CMediator::Instance()->HandleEvents(guiEvent);

    return RES_SUCCESS;
}

/*---------------------------------------------------------------*/
eResult_Code CRoomManager::BuildMaze(CMazeEvent* event)
{
    CBuildMazeEvent *buildMazeEvent     = static_cast<CBuildMazeEvent*>(event);
    m_MazeProperties.m_MazeAlgType      = buildMazeEvent->GetAlgType();
    m_MazeProperties.m_Source           = GetRandomRoom();
    m_MazeProperties.m_VisitedRoomCount = 0;
    cout<<"Source Room No= "<< m_MazeProperties.m_Source<<endl;

    CMazeThread *thread = CMazeThreadManager::Instance()->CreateMazeBuildingThread(this, event);
    if(!thread)
    {
        return RES_FAILED;
    }
    thread->Create();
    thread->Run();

    return RES_SUCCESS;
}
/*---------------------------------------------------------------*/
eResult_Code CRoomManager::FindDesitnation(CMazeEvent* event)
{
    CMazeThread *thread = CMazeThreadManager::Instance()->CreateSearchThread(this, event);
    if(!thread)
    {
        return RES_FAILED;
    }
    thread->Create();
    thread->Run();

    return RES_SUCCESS;
}

/*---------------------------------------------------------------*/
int CRoomManager::GetRandomRoom()
{
    int totalRooms = m_MazeProperties.m_Rows * m_MazeProperties.m_Cols;
    srand (time(NULL));
    int random = rand();
    int roomNo =  random % totalRooms;
    cout<<" Total Room count = " << totalRooms << " Room No = "<< roomNo<< "Random No= " << random<<endl;
    return roomNo;
}

/*---------------------------------------------------------------*/
void CRoomManager::BuildRooms()
{
    //int totalRooms = m_Rows * m_Cols;
    CRoom *room = 0;
    for (int i = 0; i < m_MazeProperties.m_Rows; i++)
    {
        for (int j = 0; j < m_MazeProperties.m_Cols; j++)
        {
            room = new CRoom((i * m_MazeProperties.m_Cols) + j, i, j );
            m_Rooms.push_back(room);
        }
    }
}

/*---------------------------------------------------------------*/
int CRoomManager::GetRandomNo(int & rndInput, int range)
{
    srand (rndInput);
    rndInput =  rand();
    return rndInput % range;
}

/*---------------------------------------------------------------*/
void CRoomManager::BuildDefaultGraph()
{
    int rndInput = time(NULL);
    int random;

    int adj;
    unsigned int totalRooms = m_Rooms.size();
    for (unsigned int i = 0; i < totalRooms; i++)
    {
        //Top
        adj = i - m_MazeProperties.m_Cols;
        if (adj >= 0)
        {
            random = GetRandomNo(rndInput, EDGE_WEIGHT_RANGE);
            m_Rooms[i]->AddEdge(adj, random);
        }
        //down
        adj = i + m_MazeProperties.m_Cols;
        if (adj < totalRooms)
        {
            random = GetRandomNo(rndInput, EDGE_WEIGHT_RANGE);
            m_Rooms[i]->AddEdge(adj, random);
        }
        //Left
        if ( (i % m_MazeProperties.m_Cols) > 0)
        {
            random = GetRandomNo(rndInput, EDGE_WEIGHT_RANGE);
            m_Rooms[i]->AddEdge(i - 1, random);
        }
        //right
        if ( (i % m_MazeProperties.m_Cols) < (m_MazeProperties.m_Cols -1) )
        {
            random = GetRandomNo(rndInput, EDGE_WEIGHT_RANGE);
            m_Rooms[i]->AddEdge(i + 1, random);
        }
    }
}

/*---------------------------------------------------------------*/
CRoom::CRoom(int roomNo,int rowNo, int colNo):m_RoomNo(roomNo), m_RowNo(rowNo), m_ColNo(colNo), m_IsVisited(false),m_OpenedWalls(0x00)
{
    cout<<"RoomNo = " << roomNo <<",RowNo = "<<rowNo <<",ColNo = "<<colNo<<" created"<<endl;
    int i = 0;
    while( i < ALG_MAX)
    {
        m_FwdPaths[i++] = 0x00;
    }
}

/*---------------------------------------------------------------*/
CRoom::~CRoom()
{
    list<AdjNode*>::iterator itr;
    itr = m_AdjacentRooms.begin();
    while (itr != m_AdjacentRooms.end())
    {
        delete *itr;
        itr = m_AdjacentRooms.erase(itr);
    }
}
/*---------------------------------------------------------------*/
 void CRoom::AddEdge(int roomNo, int distance)
 {
    m_AdjacentRooms.push_back(new AdjNode(roomNo, distance));
    //cout<<"Added Edge from "<<m_RoomNo << "==>" << roomNo<<endl;
 }

/*---------------------------------------------------------------*/
void CRoom::RemoveEdge(int roomNo)
{
    list<AdjNode*>::iterator itr;
    itr = m_AdjacentRooms.begin();
    while (itr != m_AdjacentRooms.end())
    {
        if ((*(*itr))() == roomNo)
        {
            delete *itr;
            itr = m_AdjacentRooms.erase(itr);
            cout<<" Room No = "<< roomNo << " detelted"<<endl;
            return;
        }
        else
        {
            ++itr;
        }
    }
}

/*---------------------------------------------------------------*/
bool CRoom::IsAdjContains(int roomNo)
{
    list<AdjNode*>::iterator itr;
    itr = m_AdjacentRooms.begin();
    while (itr != m_AdjacentRooms.end())
    {
        if ((*(*itr))() == roomNo)
        {
            return true;
        }
        ++itr;
    }
    return false;
}

/*---------------------------------------------------------------*/
int CRoom::GetAdjWeight(int roomNo)
{
    list<AdjNode*>::iterator itr;
    itr = m_AdjacentRooms.begin();
    while (itr != m_AdjacentRooms.end())
    {
        if ((*(*itr))() == roomNo)
        {
            return (*(*itr)).GetDistance();
        }
        ++itr;
    }
    return VAL_INFINITE;
}

/*---------------------------------------------------------------*/
bool CRoom::IsVisited()
{
    return m_IsVisited;
}


/*---------------------------------------------------------------*/
void CRoom::SetVisited(bool visisted)
{
    //cout<<"CRoom::SetVisited=> Room No = "<< m_RoomNo<<endl;
    m_IsVisited = visisted;
}

/*---------------------------------------------------------------*/
list<AdjNode*>& CRoom::GetAdjacentRoomsList()
{
    return m_AdjacentRooms;
}

/*---------------------------------------------------------------*/
void CRoom::ClearAdj()
{
    m_AdjacentRooms.clear();
}

/*---------------------------------------------------------------*/
bool CRoom::IsOpenTo(int toRoomNo)
{
    unsigned char direction = CRoomManager::Instance()->GetMoveDirection(m_RoomNo, toRoomNo);
    return (m_OpenedWalls & direction);
}

/*---------------------------------------------------------------*/
void CRoom::RemoveNotOpenedToAdj(CIManager *manager)
{
    list<AdjNode*>::iterator itr;
    itr = m_AdjacentRooms.begin();
    while (itr != m_AdjacentRooms.end())
    {
        if (!manager->GetRoom((*(*itr))())->IsVisited())
        {
            manager->GetRoom((*(*itr))())->RemoveEdge(m_RoomNo);
            delete *itr;
            itr = m_AdjacentRooms.erase(itr);
        }
        else if (!manager->GetRoom((*(*itr))())->IsOpenTo(m_RoomNo))
        {
            manager->GetRoom((*(*itr))())->RemoveEdge(m_RoomNo);
            delete *itr;
            itr = m_AdjacentRooms.erase(itr);
        }
        else
        {
            ++itr;
        }
    }
}
