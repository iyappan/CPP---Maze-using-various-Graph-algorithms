#ifndef CEVENTS_H_INCLUDED
#define CEVENTS_H_INCLUDED
#include"cmazeconst.h"

//These are the different events that can be received from User.
class CMazeEvent
{
public:
    virtual ~CMazeEvent(){}
    virtual eMaze_Event GetEventID() = 0;
    //Factory method to return its own kinds.
    static CMazeEvent* GetEvent(eMaze_Event event);
};

/*---------------------------------------------------------------*/
class CBuildRoomEvent: public CMazeEvent
{
public:
    virtual eMaze_Event GetEventID();

    void SetRows(int rows){m_Rows = rows;}
    int GetRows(){return m_Rows;}

    void SetCols(int cols){m_Cols = cols;}
    int GetCols(){return m_Cols;}

    void SetDepth(int depth){m_Depth = depth;}
    int GetDepth(){return m_Depth;}
    CBuildRoomEvent():m_Rows(DEFAULT_ROW),m_Cols(DEFAULT_COL), m_Depth(DEFAULT_DEPTH){}
private:
    int m_Rows;
    int m_Cols;
    int m_Depth;
};

/*---------------------------------------------------------------*/
class CBuildMazeEvent: public CMazeEvent
{
public:
    virtual eMaze_Event GetEventID();
    void SetAlgType(eAlg_Type algType){m_AlgType = algType;}
    eAlg_Type GetAlgType(){return m_AlgType;}
    CBuildMazeEvent():m_AlgType(ALG_DFS){}
private:
    eAlg_Type m_AlgType;
};

/*---------------------------------------------------------------*/
class CSearchDestEvent: public CMazeEvent
{
public:
    virtual eMaze_Event GetEventID();

    void SetAlgType(eAlg_Type algType){m_AlgType = algType;}
    eAlg_Type GetAlgType(){return m_AlgType;}

    void SetSource(int source){m_Source = source;}
    int GetSource(){return m_Source;}

    void SetDestination(int destination){m_Destination = destination;}
    int GetDestination(){return m_Destination;}

    CSearchDestEvent(): m_AlgType(ALG_DFS), m_Source(DEFAULT_SRC), m_Destination(DEFAULT_DST){}

private:
    eAlg_Type m_AlgType;
    int m_Source;
    int m_Destination;
};

/*---------------------------------------------------------------*/
class CStopEvent:public CMazeEvent
{
public:
    virtual eMaze_Event GetEventID();
    CStopEvent(){}
};

/*---------------------------------------------------------------*/

#endif // CEVENTS_H_INCLUDED
