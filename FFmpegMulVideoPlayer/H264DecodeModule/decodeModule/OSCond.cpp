#include "OSCond.h"
#include "OSMutex.h"
#include "OSThread.h"
#if defined(__WIN32__) || defined(_WIN32) || defined(_WIN32_WCE)
#else
#include <sys/time.h>
#endif


OSCond::OSCond()
{
#if defined(__WIN32__) || defined(_WIN32) || defined(_WIN32_WCE)
    fCondition = ::CreateEvent(NULL, FALSE, FALSE, NULL);
#else
    pthread_condattr_t cond_attr;
    pthread_condattr_init(&cond_attr);
    int ret = pthread_cond_init(&fCondition, &cond_attr);
#endif
}

OSCond::~OSCond()
{
#if defined(__WIN32__) || defined(_WIN32) || defined(_WIN32_WCE)
    bool theErr = ::CloseHandle(fCondition);
#else
    pthread_cond_destroy(&fCondition);
#endif
}
#if defined(__WIN32__) || defined(_WIN32) || defined(_WIN32_WCE)
#else
void OSCond::TimedWait(OSMutex* inMutex, unsigned int inTimeoutInMilSecs)
{
    struct timespec ts;
    struct timeval tv;
    struct timezone tz;
    int sec, usec;
    
    if (inTimeoutInMilSecs == 0)
        (void)pthread_cond_wait(&fCondition,inMutex->getMutex());
    else
    {
        gettimeofday(&tv, &tz);
        sec = inTimeoutInMilSecs / 1000;
        inTimeoutInMilSecs = inTimeoutInMilSecs - (sec * 1000);
        usec = inTimeoutInMilSecs * 1000;
        ts.tv_sec = tv.tv_sec + sec;
        ts.tv_nsec = (tv.tv_usec + usec) * 1000;
        if(ts.tv_nsec > 999999999)
        {
             ts.tv_sec++;
             ts.tv_nsec -= 1000000000;
        }
        (void)pthread_cond_timedwait(&fCondition,inMutex->getMutex(), &ts);
    }
}
#endif
