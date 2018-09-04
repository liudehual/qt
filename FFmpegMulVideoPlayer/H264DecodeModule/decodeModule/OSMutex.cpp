/* 
============================================================================================
    Locker 快速锁对象 实现文件

    add 2016.09.23 by GCT

    Copyright (c), ...

=============================================================================================
*/

#include "OSMutex.h"
OSMutex::OSMutex()
{
#if defined(__WIN32__) || defined(_WIN32)
    InitializeCriticalSection( &m_mutex);
#else
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&m_mutex,&attr);
#endif
}

OSMutex::~OSMutex()
{
#if defined(__WIN32__) || defined(_WIN32)
    DeleteCriticalSection( &m_mutex );
#else
    pthread_mutex_destroy( &m_mutex );
#endif
}

void OSMutex::Lock()
{
#if defined(__WIN32__) || defined(_WIN32)
    EnterCriticalSection(&m_mutex);
#else
    pthread_mutex_lock( &m_mutex );
#endif
}
void OSMutex::Unlock()
{
#if defined(__WIN32__) || defined(_WIN32)
    LeaveCriticalSection( &m_mutex );
#else
    pthread_mutex_unlock( &m_mutex );
#endif
}
