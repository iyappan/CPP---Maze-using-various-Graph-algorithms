#ifndef CMAZETHREAD_H_INCLUDED
#define CMAZETHREAD_H_INCLUDED

#include <wx/thread.h>
#include"cmazeconst.h"

/***************************************************************************************
CMazeThread will be created by CMazeThreadManager. And the CMazeThread Object will
be deleted by its own after it complete the tasks.
***************************************************************************************/
class CIManager;
class CMazeThread : public wxThread
{
    public:
        CMazeThread(CIManager *manager);
        ~CMazeThread();
        void SetAlgType(eAlg_Type algType){m_AlgType = algType;}
        void SetIsSearch(bool isSearch){m_IsSearch = isSearch;}
        void SetSource(int source){m_Source = source;}
        void SetDestination(int destination){m_Destination = destination;}
    private:
        eAlg_Type m_AlgType;
        bool m_IsSearch;
        int m_Source;
        int m_Destination;
        CIManager* m_Manager;
        void* Entry();
};

/*---------------------------------------------------------------*/
class CMazeEvent;
class CMazeThreadManager
{
public:
    CMazeThread* CreateSearchThread(CIManager *manager,  CMazeEvent* event);
    CMazeThread* CreateMazeBuildingThread(CIManager *manager,  CMazeEvent* event);
    void ClearAllThreads();

    static CMazeThreadManager* Instance();

private:
    CMazeThreadManager();
    static CMazeThreadManager* m_Instance;
};
#endif // CMAZETHREAD_H_INCLUDED
