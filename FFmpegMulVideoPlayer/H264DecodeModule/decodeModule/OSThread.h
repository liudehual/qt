// OSThread.h
#ifndef __OSTHREAD__
#define __OSTHREAD__

#if defined(__WIN32__) || defined(_WIN32) || defined(_WIN32_WCE)
#include <windows.h>
#if 1
#include <process.h>
#include <io.h>
#include <direct.h>
#endif
#include <errno.h>
#include <Mmsystem.h>
#pragma comment(lib, "Winmm.lib") 
#else
    #include <errno.h>
    #include <sys/errno.h>
    #include <pthread.h>
#endif
#include <string.h>
class OSThread
{

public:
                //
                // Call before calling any other OSThread function
                static void     Initialize();
                
                                OSThread();
    virtual                     ~OSThread();
    
    //
    // Derived classes must implement their own entry function
    virtual     void            Entry() = 0;
                void            Start();
                
                static void     ThreadYield();
                static void     Sleep(unsigned int inMsec);
                
                void            Join();
                void            SendStopRequest() { fStopRequested = true; }
                unsigned short          IsStopRequested() { return fStopRequested; }
                void            StopAndWaitForThread();

                void*           GetThreadData()         { return fThreadData; }
                void            SetThreadData(void* inThreadData) { fThreadData = inThreadData; }
                
                // As a convienence to higher levels, each thread has its own date buffer
               
                static void*    GetMainThreadData()     { return sMainThreadData; }
                static void     SetMainThreadData(void* inData) { sMainThreadData = inData; }
                static void     SetUser(char *user) {::strncpy(sUser,user, sizeof(sUser) -1); sUser[sizeof(sUser) -1]=0;} 
                static void     SetGroup(char *group) {::strncpy(sGroup,group, sizeof(sGroup) -1); sGroup[sizeof(sGroup) -1]=0;} 
                static void     SetPersonality(char *user, char* group) { SetUser(user); SetGroup(group); };
                unsigned short          SwitchPersonality();
#if DEBUG
                unsigned int          GetNumLocksHeld() { return 0; }
                void            IncrementLocksHeld() {}
                void            DecrementLocksHeld() {}
#endif

#if __linux__ ||  __MacOSX__
                static void     WrapSleep( unsigned short wrapSleep) {sWrapSleep = wrapSleep; }
#endif

#if defined(__WIN32__) || defined(_WIN32) || defined(_WIN32_WCE)
    static int          GetErrno();
    static unsigned long        GetCurrentThreadID() { return ::GetCurrentThreadId(); }
#else
    static  int         GetErrno() { return errno; }
    static  pthread_t   GetCurrentThreadID() { return ::pthread_self(); }
#endif

    static  OSThread*   GetCurrent();
    
private:
	static unsigned long long Microseconds();


#if defined(__WIN32__) || defined(_WIN32) || defined(_WIN32_WCE)
    static unsigned long    sThreadStorageIndex;
#else
    static pthread_key_t    gMainKey;
#ifdef _POSIX_THREAD_PRIORITY_SCHEDULING
    static pthread_attr_t sThreadAttr;
#endif
#endif

   static char sUser[128];
   static char sGroup[128];
    

    unsigned short fStopRequested;
    unsigned short fJoined;

#if defined(__WIN32__) || defined(_WIN32) || defined(_WIN32_WCE)
    HANDLE          fThreadID;
#else
    pthread_t       fThreadID;
#endif
    void*           fThreadData;
    
    static void*    sMainThreadData;
#if  defined(__WIN32__) || defined(_WIN32) || defined(_WIN32_WCE)
    static unsigned int WINAPI _Entry(LPVOID inThread);
#else
    static void*    _Entry(void* inThread);
#endif

#if __linux__ || __MacOSX__
    static unsigned short sWrapSleep;
#endif


};
#endif
