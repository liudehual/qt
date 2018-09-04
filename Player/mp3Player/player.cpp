#include "player.h"
#include "widget.h"
#include "readthread.h"
Player::Player()
{
   fReadThread=new ReadThread(this,this);
   fMainWidget=new Widget(this,this);
}
Player::~Player()
{
}
void Player::start()
{
    fMainWidget->show();
}
