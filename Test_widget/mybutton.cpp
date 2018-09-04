#include "mybutton.h"

MyButton::MyButton()
{
    a=0;
}

void MyButton::myAddClieckedSlot(bool)
{
    emit myAddClieckedSignal(++a);
}
void MyButton::mySubClieckedSlot(bool)
{
    if(a<=0) return;
    emit mySubClieckedSignal(--a);
}
