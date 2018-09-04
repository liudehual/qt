#ifndef _OSCOND_H_
#define _OSCOND_H_
#if defined(__WIN32__) || defined(_WIN32) || defined(_WIN32_WCE)
	#include <windows.h>
    #include <process.h>
#else
    #include <pthread.h>
#endif

#include "OSMutex.h"
class OSCond 
{
    public:

        OSCond();
        ~OSCond();
        
        inline void     Signal();
        inline void     Wait(OSMutex* inMutex, unsigned int inTimeoutInMilSecs = 0);
        inline void     Broadcast();

    private:

#if defined(__WIN32__) || defined(_WIN32) || defined(_WIN32_WCE)
        HANDLE              fCondition;
        unsigned int              fWaitCount;
#else
        pthread_cond_t      fCondition;
        void                TimedWait(OSMutex* inMutex, unsigned int inTimeoutInMilSecs);
#endif
};

inline void OSCond::Wait(OSMutex* inMutex, unsigned int inTimeoutInMilSecs)
{ 
#if defined(__WIN32__) || defined(_WIN32) || defined(_WIN32_WCE)
    unsigned long theTimeout = INFINITE;
    if (inTimeoutInMilSecs > 0)
        theTimeout = inTimeoutInMilSecs;
    inMutex->Unlock();
    fWaitCount++;
    DWORD theErr = ::WaitForSingleObject(fCondition, theTimeout);
    fWaitCount--;
    inMutex->Lock();
#else
    this->TimedWait(inMutex, inTimeoutInMilSecs);
#endif
}

inline void OSCond::Signal()
{
#if defined(__WIN32__) || defined(_WIN32) || defined(_WIN32_WCE)
    bool theErr = ::SetEvent(fCondition);
#else
    pthread_cond_signal(&fCondition);
#endif
}

inline void OSCond::Broadcast()
{
#if defined(__WIN32__) || defined(_WIN32) || defined(_WIN32_WCE)
    unsigned int waitCount = fWaitCount;
    for (unsigned int x = 0; x < waitCount; x++)
    {
        BOOL theErr = ::SetEvent(fCondition);
    }
#else
    pthread_cond_broadcast(&fCondition);
#endif
}

#endif //_OSCOND_H_
