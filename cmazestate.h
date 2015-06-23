#ifndef CMAZESTATE_H_INCLUDED
#define CMAZESTATE_H_INCLUDED

#include"cmazeconst.h"

class CMaze;
class CMazeEvent;
class CIManager;
class CMazeState
{
public:
    virtual eResult_Code PreHandle(CIManager *manager, CMazeEvent* event) = 0;
    virtual eResult_Code BuildRooms(CIManager *manager, CMazeEvent* event) = 0;
    virtual eResult_Code BuildMaze(CIManager *manager, CMazeEvent* event) = 0;
    virtual eResult_Code SearchDestination(CIManager *manager, CMazeEvent* event) = 0;
    virtual eResult_Code Stop(CIManager *manager, CMazeEvent* event) = 0;

    static void InitState();
    static CMazeState* GetMazeState(eMaze_States state);

protected:
    CMazeState(){}
    static CMazeState *m_States[STATE_MAX];
};

/*---------------------------------------------------------------*/
class COnIdeal: public CMazeState
{
public:
    COnIdeal(){}
    eResult_Code PreHandle(CIManager *manager, CMazeEvent* event);
    eResult_Code BuildRooms(CIManager *manager, CMazeEvent* event);
    eResult_Code BuildMaze(CIManager *manager, CMazeEvent* event);
    eResult_Code SearchDestination(CIManager *manager, CMazeEvent* event);
    eResult_Code Stop(CIManager *manager, CMazeEvent* event);

};

/*---------------------------------------------------------------*/
class COnConstruction: public CMazeState
{
public:
    COnConstruction(){}
    eResult_Code PreHandle(CIManager *manager, CMazeEvent* event);
    eResult_Code BuildRooms(CIManager *manager, CMazeEvent* event);
    eResult_Code BuildMaze(CIManager *manager, CMazeEvent* event);
    eResult_Code SearchDestination(CIManager *manager, CMazeEvent* event);
    eResult_Code Stop(CIManager *manager, CMazeEvent* event);

};

/*---------------------------------------------------------------*/
class COnWait: public CMazeState
{
 public:
    COnWait(){}
    eResult_Code PreHandle(CIManager *manager, CMazeEvent* event);
    eResult_Code BuildRooms(CIManager *manager, CMazeEvent* event);
    eResult_Code BuildMaze(CIManager *manager, CMazeEvent* event);
    eResult_Code SearchDestination(CIManager *manager, CMazeEvent* event);
    eResult_Code Stop(CIManager *manager, CMazeEvent* event);
};

/*---------------------------------------------------------------*/
class COnTravel: public CMazeState
{
public:
    COnTravel(){}
    eResult_Code PreHandle(CIManager *manager, CMazeEvent* event);
    eResult_Code BuildRooms(CIManager *manager, CMazeEvent* event);
    eResult_Code BuildMaze(CIManager *manager, CMazeEvent* event);
    eResult_Code SearchDestination(CIManager *manager, CMazeEvent* event);
    eResult_Code Stop(CIManager *manager, CMazeEvent* event);
};
#endif // CMAZESTATE_H_INCLUDED
