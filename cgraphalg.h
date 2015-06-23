#ifndef CGRAPHALG_H_INCLUDED
#define CGRAPHALG_H_INCLUDED
#include<vector>
#include<queue>

#include"cmazeconst.h"
/***************************************************************************************
CGraphAlg is the base class to expose interface for various Graph algorithms. Like
DFS(Depth First Search), BFS(Breath First Search), Dijistra shortest path and so on.
If anybody wants to add a new algorithm here it needs to be added.
***************************************************************************************/
using namespace std;
class CIManager;
class CGraphAlg
{
public:
    virtual ~CGraphAlg(){}
    virtual eResult_Code BuildMaze(CIManager *manager, int sourceRoomNo) {return RES_SUCCESS;}
    virtual eResult_Code SearchPath(CIManager *manager, int sourceRoomNo, int DestRoomNo) = 0;
    static CGraphAlg* GetAlg(eAlg_Type algType);

    virtual string GetPseudocode() = 0;

protected:
    CGraphAlg(){m_IsReachedDest = false;}
    void SubmitDrawEvent(eMaze_Event eventType, CIManager *manager, int sourceRoomNo, int DestRoomNo, eAlg_Type algType);

    bool m_IsReachedDest;
};

/*---------------------------------------------------------------*/
class CDFSAlg : public CGraphAlg
{
public:
    eResult_Code BuildMaze(CIManager *manager, int sourceRoomNo);
    eResult_Code SearchPath(CIManager *manager, int sourceRoomNo, int DestRoomNo);

    string GetPseudocode();
    CDFSAlg(){}
    ~CDFSAlg(){}
};

/*---------------------------------------------------------------*/
class CBFSAlg : public CGraphAlg
{
public:
    eResult_Code BuildMaze(CIManager *manager, int sourceRoomNo);
    eResult_Code SearchPath(CIManager *manager, int sourceRoomNo, int DestRoomNo);

    string GetPseudocode();
    CBFSAlg(){}
    ~CBFSAlg(){}
};

/*---------------------------------------------------------------*/
class CDijkstraAlg : public CGraphAlg
{
public:
    eResult_Code SearchPath(CIManager *manager, int sourceRoomNo, int DestRoomNo);

    string GetPseudocode();
    CDijkstraAlg(){}
    ~CDijkstraAlg(){}
};

/*---------------------------------------------------------------*/
class CBellmanFordAlg : public CGraphAlg
{
public:
    eResult_Code SearchPath(CIManager *manager, int sourceRoomNo, int DestRoomNo);
    string GetPseudocode();

    CBellmanFordAlg(){}
    ~CBellmanFordAlg(){}
};

/*---------------------------------------------------------------*/
class CFloydWarshallAlg : public CGraphAlg
{
public:
    eResult_Code SearchPath(CIManager *manager, int sourceRoomNo, int DestRoomNo);
    string GetPseudocode();

    CFloydWarshallAlg(){}
    ~CFloydWarshallAlg(){}
};
#endif // CGRAPHALG_H_INCLUDED
