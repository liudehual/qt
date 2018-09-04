#ifndef PLAYER_H
#define PLAYER_H
#include "metadata.h"
class ReadThread;
class Widget;
class Player
{

public:
    Player();
    ~Player();
    void start();
    Widget *getWidget(){return fMainWidget;}
    ReadThread *getReadThread(){return fReadThread;}
private:
    Widget *fMainWidget;
    ReadThread *fReadThread;
};

#endif // PLAYER_H
