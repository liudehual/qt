/* 
============================================================================================
	跨平台锁实现

	add 2016.09.23 by GCT

	Copyright (c), ...	

=============================================================================================
*/

#ifndef _OSMUTEX_LOCKER_H
#define _OSMUTEX_LOCKER_H

#if defined(__WIN32__) || defined(_WIN32)
#include <windows.h>
#include <process.h>
#else
#include <pthread.h>
#endif


class OSMutex
{
public:
	OSMutex();
	~OSMutex();

	void Lock();
	void Unlock();

#if defined(__WIN32__) || defined(_WIN32) || defined(_WIN32_WCE)
    CRITICAL_SECTION * getMutex(){return &m_mutex;}
#else
    pthread_mutex_t * getMutex(){return	&m_mutex;}
#endif
protected:
#if defined(__WIN32__) || defined(_WIN32) || defined(_WIN32_WCE)
    CRITICAL_SECTION	m_mutex;
#else
	pthread_mutex_t 	m_mutex;			// 互斥锁
#endif
};


class   OSMutexLocker
{
    public:

        OSMutexLocker(OSMutex *inMutexP) : fMutex(inMutexP) { if (fMutex != NULL) fMutex->Lock(); }
        ~OSMutexLocker() {  if (fMutex != NULL) fMutex->Unlock(); }

        void Lock()         { if (fMutex != NULL) fMutex->Lock(); }
        void Unlock()       { if (fMutex != NULL) fMutex->Unlock(); }

    private:

        OSMutex*    fMutex;
};
#endif
