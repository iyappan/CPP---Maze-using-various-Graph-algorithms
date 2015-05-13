#include"cmazethread.h"
#include"croommanager.h"
#include"cgraphalg.h"
#include"cevents.h"
CMazeThread::CMazeThread(CIManager *manager) : m_Manager(manager)
{

}

/*---------------------------------------------------------------*/
CMazeThread::~CMazeThread()
{
if (m_IsSearch){}
}
/*---------------------------------------------------------------*/
void* CMazeThread::Entry()
{
    CGraphAlg *alg = CGraphAlg::GetAlg(m_AlgType);
    if (m_IsSearch)
    {
        m_Manager->ClearVisitedStatus();
        //Applying the selected graph algorithm on the builded maze graph.
        eResult_Code result  = alg->SearchPath(m_Manager, m_Source, m_Destination);
    }
    else
    {
        //Applying the selected graph algorithm to build the maze.
        eResult_Code result  = alg->BuildMaze(m_Manager, m_Manager->GetSource());
    }
    delete alg;
}
/*---------------------------------------------------------------*/
CMazeThreadManager* CMazeThreadManager::m_Instance = 0;
CMazeThreadManager::CMazeThreadManager()
{
}

/*---------------------------------------------------------------*/
CMazeThreadManager* CMazeThreadManager::Instance()
{
    if (!m_Instance)
    {
        m_Instance = new CMazeThreadManager();
    }
    return m_Instance;

}
/*---------------------------------------------------------------*/
CMazeThread* CMazeThreadManager::CreateSearchThread(CIManager *manager, CMazeEvent* event )
{
    CSearchDestEvent *searchDestEvent = static_cast<CSearchDestEvent*>(event);
    eAlg_Type algType = searchDestEvent->GetAlgType();
    //This thread object will be deleted its own.
    CMazeThread *thread = new CMazeThread(manager);
    thread->SetAlgType(algType);
    thread->SetIsSearch(true);
    thread->SetSource(searchDestEvent->GetSource());
    thread->SetDestination(searchDestEvent->GetDestination());
    return thread;
}

/*---------------------------------------------------------------*/
CMazeThread* CMazeThreadManager::CreateMazeBuildingThread(CIManager *manager, CMazeEvent* event )
{
    CBuildMazeEvent *buildMazeEvent = static_cast<CBuildMazeEvent*>(event);
    //This thread object will be deleted its own.
    CMazeThread *thread = new CMazeThread(manager);
    thread->SetAlgType(buildMazeEvent->GetAlgType());
    thread->SetIsSearch(false);
    return thread;
}

/*---------------------------------------------------------------*/

