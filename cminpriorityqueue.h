#ifndef CMINPRIORITYQUEUE_H_INCLUDED
#define CMINPRIORITYQUEUE_H_INCLUDED
#include"cmazeconst.h"
#include<map>

/***************************************************************************************
This container provides support to get the first minimum key value and update the key.
Or we can say it provide the support of min priority queue.
***************************************************************************************/
using namespace std;
template<typename T1, typename T2>
class min_priority_queue
{
public:
    void add_with_priority(T1 val1, T2 val2);
    bool decrease_priority(T1 val1, T2 val2);
    T2 extract_min();

    bool isEmpty();
    int count(T1 val1);
    int size();

private:
    multimap<T1, T2> m_Queue;
};

/*---------------------------------------------------------------*/
template<typename T1, typename T2>
void min_priority_queue<T1, T2>::add_with_priority(T1 val1, T2 val2)
{
    m_Queue.insert(pair<T1,T2>(val1, val2));
}

/*---------------------------------------------------------------*/
template<typename T1, typename T2>
T2 min_priority_queue<T1, T2>::extract_min()
{
    typename multimap<T1, T2>::iterator itr1 = m_Queue.begin();
    typename multimap<T1, T2>::iterator itr2;
    pair<typename multimap<T1, T2>::iterator, typename multimap<T1, T2>::iterator> pairRange;

    T2 ret = VAL_INFINITE;
    if (itr1 != m_Queue.end())
    {
        pairRange = m_Queue.equal_range((*itr1).first);
        itr2 = pairRange.first;
        if (itr2 != pairRange.second)
        {
            ret = itr2->second;
            m_Queue.erase(itr2);
        }
    }
    return ret;
}

/*---------------------------------------------------------------*/
template<typename T1, typename T2>
bool min_priority_queue<T1, T2>::decrease_priority(T1 val1, T2 val2)
{
    typename multimap<T1, T2>::iterator itr1 = m_Queue.begin();
    typename multimap<T1, T2>::iterator itr2;
    pair<typename multimap<T1, T2>::iterator, typename multimap<T1, T2>::iterator> range;
    bool isRemoved = false; // Before updated remove the existing same element then insert with the new value.
    while (itr1 != m_Queue.end() && !isRemoved)
    {
        range = m_Queue.equal_range((*itr1).first);
        itr2 = range.first;
        while (itr2 != range.second)
        {
            if((*itr2).second == val2)
            {
                m_Queue.erase(itr2);
                isRemoved = true;
                break;
            }
            ++itr2;
        }
        ++itr1;
    }

    if (isRemoved)
    {
        m_Queue.insert(std::pair<T1,T2>(val1, val2));
    }
    return isRemoved;
}

/*---------------------------------------------------------------*/
template<typename T1, typename T2>
bool min_priority_queue<T1, T2>::isEmpty()
{
    return m_Queue.empty();
}

/*---------------------------------------------------------------*/
template<typename T1, typename T2>
int  min_priority_queue<T1, T2>::size()
{
   return m_Queue.size();
}

/*---------------------------------------------------------------*/
template<typename T1, typename T2>
int  min_priority_queue<T1, T2>::count(T1 val1)
{
   return m_Queue.count(val1);
}

#endif // CMINPRIORITYQUEUE_H_INCLUDED
