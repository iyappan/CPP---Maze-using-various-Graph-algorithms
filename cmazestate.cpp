#include"cmazestate.h"
#include"cmaze.h"
#include"croommanager.h"
#include"cevents.h"

#include<iostream>
using namespace std;

CMazeState *CMazeState::m_States[] = {0, 0, 0, 0}; // Initialize with NULL for the count of STATE_MAX;
void CMazeState::InitState()
{
    m_States[STATE_IDLE]         = new COnIdeal();
    m_States[STATE_CONSTRUCTION] = new COnConstruction();
    m_States[STATE_WAIT]         = new COnWait();
    m_States[STATE_TRAVEL]       = new COnTravel();
}
CMazeState* CMazeState::GetMazeState(eMaze_States state)
{
    return m_States[state];
}

eResult_Code COnIdeal::PreHandle(CIManager *manager, CMazeEvent* event)
{
    return RES_SUCCESS;
}

eResult_Code COnIdeal::BuildRooms(CIManager *manager, CMazeEvent* event)
{
    cout<<"COnIdeal::BuildRooms"<<endl;
    manager->InitMazeRooms(event);
    manager->ChangeState(STATE_CONSTRUCTION);
    return RES_SUCCESS;
}

eResult_Code COnIdeal::BuildMaze(CIManager *manager, CMazeEvent* event)
{

    return RES_SUCCESS;
}

eResult_Code COnIdeal::SearchDestination(CIManager *manager, CMazeEvent* event)
{
    return RES_SUCCESS;
}

eResult_Code COnIdeal::Stop(CIManager *manager, CMazeEvent* event)
{
    manager->ClearRooms();
    manager->ChangeState(STATE_IDLE);
    return RES_SUCCESS;
}

/*---------------------------------------------------------------*/
eResult_Code COnConstruction::PreHandle(CIManager *manager, CMazeEvent* event)
{
    return RES_SUCCESS;
}

eResult_Code COnConstruction::BuildRooms(CIManager *manager, CMazeEvent* event)
{
    return RES_SUCCESS;
}

eResult_Code COnConstruction::BuildMaze(CIManager *manager, CMazeEvent* event)
{
    cout<<"COnConstruction::BuildMaze"<<endl;
    manager->BuildMaze(event);
    manager->ChangeState(STATE_WAIT);
    return RES_SUCCESS;
}

eResult_Code COnConstruction::SearchDestination(CIManager *manager, CMazeEvent* event)
{
    return RES_SUCCESS;
}

eResult_Code COnConstruction::Stop(CIManager *manager, CMazeEvent* event)
{
    manager->ClearRooms();
    manager->ChangeState(STATE_IDLE);
    return RES_SUCCESS;
}

/*---------------------------------------------------------------*/
eResult_Code COnWait::PreHandle(CIManager *manager, CMazeEvent* event)
{
    return RES_SUCCESS;
}

eResult_Code COnWait::BuildRooms(CIManager *manager, CMazeEvent* event)
{
    return RES_SUCCESS;
}

eResult_Code COnWait::BuildMaze(CIManager *manager, CMazeEvent* event)
{
    return RES_SUCCESS;
}

eResult_Code COnWait::SearchDestination(CIManager *manager, CMazeEvent* event)
{
    cout<<"\nCOnWait::SearchDestination"<<endl;
    manager->FindDesitnation(event);
    manager->ChangeState(STATE_WAIT);
    return RES_SUCCESS;
}

eResult_Code COnWait::Stop(CIManager *manager, CMazeEvent* event)
{
    manager->ClearRooms();
    manager->ChangeState(STATE_IDLE);
    return RES_SUCCESS;

}

/*---------------------------------------------------------------*/
eResult_Code COnTravel::PreHandle(CIManager *manager, CMazeEvent* event)
{
    return RES_SUCCESS;
}

eResult_Code COnTravel::BuildRooms(CIManager *manager, CMazeEvent* event)
{
    return RES_SUCCESS;
}

eResult_Code COnTravel::BuildMaze(CIManager *manager, CMazeEvent* event)
{
    return RES_SUCCESS;
}

eResult_Code COnTravel::SearchDestination(CIManager *manager, CMazeEvent* event)
{
    return RES_SUCCESS;
}

eResult_Code COnTravel::Stop(CIManager *manager, CMazeEvent* event)
{
    manager->ClearRooms();
    manager->ChangeState(STATE_IDLE);
    return RES_SUCCESS;

}
/*---------------------------------------------------------------*/
