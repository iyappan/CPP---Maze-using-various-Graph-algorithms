#ifndef CROOMMANAGER_H_INCLUDED
#define CROOMMANAGER_H_INCLUDED
#include<vector>
#include<list>

#include"cmazeconst.h"

using namespace std;

/*Properties of Maze board*/
typedef struct SMazeProperties
{
    int m_Rows;              //Number of rows in the Maze board.
    int m_Cols;              //Number of cols in the Maze board.

    int m_Source;            //Randomly picked source room, where maze start building.
    int m_Destination;       //Dynamically generated destination room.

    int m_VisitedRoomCount;  //Total number of visited rooms in the Maze board, during the room visit.

    eAlg_Type m_MazeAlgType; //Which algorithm to choose during maze building.
    int m_Depth;             //How depth the visit should go. Mostly used in Depth first search.

    SMazeProperties();
}SMazeProperties;

/*---------------------------------------------------------------*/
//CIManager: Its kinds can do various tasks on rooms. It can be on core
//           room logic and drawings on GUI rooms.
/*---------------------------------------------------------------*/
class CRoom;
class CMazeEvent;
class CIManager
{
public:
    CIManager();
    virtual ~CIManager();

    virtual eResult_Code HandleEvents(CMazeEvent* event)    = 0;
    virtual eResult_Code InitMazeRooms(CMazeEvent* event)   = 0;
    virtual eResult_Code BuildMaze(CMazeEvent* event)       = 0;
    virtual eResult_Code FindDesitnation(CMazeEvent* event) = 0;

    void SetBuildMazeAlgType(eAlg_Type mazeAlgType){m_MazeProperties.m_MazeAlgType = mazeAlgType;}
    eAlg_Type GetBuildMazeAlgType(){return m_MazeProperties.m_MazeAlgType;}

    void SetDestination(int destination){m_MazeProperties.m_Destination = destination;}
    int GetDestination(){return m_MazeProperties.m_Destination;}

    int GetSource(){return m_MazeProperties.m_Source;}

    void SetVisitedRoomCout(int visitedRoomCount){m_MazeProperties.m_VisitedRoomCount = visitedRoomCount;}
    int& GetVisitedRoomCount(){return m_MazeProperties.m_VisitedRoomCount;}

    void SetRows(int rows){m_MazeProperties.m_Rows = rows;}
    int GetRows(){return m_MazeProperties.m_Rows;}

    void SetCols(int cols){m_MazeProperties.m_Cols = cols;}
    int GetCols(){return m_MazeProperties.m_Cols;}

    void SetDepth(int depth){m_MazeProperties.m_Depth = depth;}
    int GetDepth(){return m_MazeProperties.m_Depth;}

    void PrintMaze();
    void ClearRooms();
    void ClearVisitedStatus();
    void FinalizeDestinationRoom(int roomNo);

    CRoom* GetRoom(int roomNo);
    int GetRoomCounts();
    unsigned char GetMoveDirection(int srcRoomNo, int DestRoomNo);


    static SMazeProperties& GetMazeProperties();
    static vector<CRoom*>& GetRooms();

protected:
    //These two static properties will be used by the concrete classes.
    static SMazeProperties m_MazeProperties;
    static vector<CRoom*> m_Rooms;  //It contains all the rooms in the maze board.
};

/*---------------------------------------------------------------*/
class CRoomManager : public CIManager
{
public:
    virtual eResult_Code HandleEvents(CMazeEvent* event);
    static CRoomManager* Instance();

protected:
    //Generate a graph for the default rooms.
    virtual eResult_Code InitMazeRooms(CMazeEvent* event);
    //Building the maze on the given rooms(graph).
    virtual eResult_Code BuildMaze(CMazeEvent* event);
    //Travels over builded rooms(graph) to find the destination.
    virtual eResult_Code FindDesitnation(CMazeEvent* event);

private:
    CRoomManager();
    //Generate the random room number to build the Maze
    int GetRandomRoom();
    void BuildRooms();
    void BuildDefaultGraph();

    int GetRandomNo(int &rndInput, int range = 50);

    static CRoomManager *m_Instance;

};


/*---------------------------------------------------------------*/
class AdjNode
{
public:
    AdjNode(int adjRoomNo, int distance): m_AdjRoomNo(adjRoomNo), m_Distance(distance){}
    int operator()(){return m_AdjRoomNo;}

    void SetAdjRoomNo(int adjRoomNo){m_AdjRoomNo = adjRoomNo;}
    void SetDistance(int distance){m_Distance = distance;}

    int GetAdjRoomNo(){return m_AdjRoomNo;}
    int GetDistance(){return m_Distance;}

private:
    int m_AdjRoomNo;
    int m_Distance;
};

class CRoom
{
public:
    CRoom(int roomNo,int rowNo, int colNo);
    ~CRoom();

    void AddEdge(int roomNo, int distance);
    void RemoveEdge(int roomNo);

    bool IsVisited();
    void SetVisited(bool visited = true);

    int GetRowNo(){return m_RowNo;}
    int GetColNo(){return m_ColNo;}

    int GetRoomNo(){return m_RoomNo;}
    list<AdjNode*>& GetAdjacentRoomsList();

    bool IsAdjContains(int roomNo);
    int GetAdjWeight(int roomNo);

    unsigned char GetOpendWalls(){return m_OpenedWalls;}
    void UpdateOpendWalls(unsigned char updateOpenWalls){m_OpenedWalls = m_OpenedWalls | updateOpenWalls;}
    void ClearAdj();
    bool IsOpenTo(int toRoomNo);
    void RemoveNotOpenedToAdj(CIManager *manager);

    unsigned char GetFwdPaths(eAlg_Type algType){return m_FwdPaths[algType];}
    void UpdateFwdPaths(eAlg_Type algType, unsigned char updateFwdPath){m_FwdPaths[algType] = m_FwdPaths[algType] | updateFwdPath;}
private:
    int m_RoomNo;
    int m_RowNo;
    int m_ColNo;
    bool m_IsVisited;
    list<AdjNode*> m_AdjacentRooms;

    //GUI Properties
    /*
        m_OpenedWalls:
        00000000->All walls are closed
        00001000->Left wall is opened
        00000100->Right wall is opened
        00000010->Top wall is opened
        00000001->Bottom wall is opened
        ..
        00001111->All walls are opened.

        m_FwdPaths:
        00000000->There is no traveled path
        00001000->Traveled via Left
        00000100->Traveled via Right
        00000010->Traveled via Top
        00000001->Traveled via Bottom
        ..
        00001111->Traveled via all the paths.
    */
    unsigned char m_OpenedWalls;
    unsigned char m_FwdPaths[ALG_MAX];
};
#endif // CROOMMANAGER_H_INCLUDED
