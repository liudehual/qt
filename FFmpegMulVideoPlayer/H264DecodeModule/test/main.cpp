#include <iostream>
#include "Module.h"
#include "Log.h"
#include <stdio.h>
#include <stdlib.h>
#include "OSThread.h"
using namespace std;

class testClient
{
public:
  static void handler(void *arg,char *buf,int width,int height);
};
void testClient::handler(void *arg,char *buf,int width,int height)
{
    fprintf(stderr,"width %d height %d \n",width,height);
}
int main()
{

  Module::initOS();

  testClient theClient;

  Module *theModule= Module::createNewModule();
  theModule->initModule((void *)&theClient,testClient::handler);
  theModule->setFileName("test.264");
  theModule->startModule();

 // OSThread::Sleep(2000);
  Module *theModule1= Module::createNewModule();
  theModule1->initModule((void *)&theClient,testClient::handler);
  theModule1->setFileName("test.264");
  theModule1->startModule();

  getchar();
  return 0;
}
