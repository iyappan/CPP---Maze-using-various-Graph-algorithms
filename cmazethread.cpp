#include"cmazethread.h"
#include"croommanager.h"
#include"cgraphalg.h"
#include"cevents.h"
#include"guievent.h"
#include"cmaze.h"
CMazeThread::CMazeThread(CIManager *manager) : m_Manager(manager)
{

}

/*---------------------------------------------------------------*/
CMazeThread::~CMazeThread()
{
    GUINotifEvent *notifEvent = static_cast<GUINotifEvent*>(GUIMazeEvent::GetEvent(EV_GUI_NOTIF));
    if (!m_IsSearch)
    {

        notifEvent->SetNotif(MAZE_BUILD_COMPLETED);

    }
    else
    {
        notifEvent->SetNotif(SEARCH_COMPLETED);
    }
    CMediator::Instance()->HandleEvents(notifEvent);
}
/*---------------------------------------------------------------*/
void* CMazeThread::Entry()
{
    CGraphAlg *alg = CGraphAlg::GetAlg(m_AlgType);
    GUINotifEvent *notifEvent = static_cast<GUINotifEvent*>(GUIMazeEvent::GetEvent(EV_GUI_NOTIF));
    notifEvent->SetNotif(DISPLAY_DATA3);
    notifEvent->SetData(alg->GetPseudocode());
    CMediator::Instance()->HandleEvents(notifEvent);

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

