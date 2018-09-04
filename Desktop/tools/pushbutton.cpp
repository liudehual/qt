#include "pushbutton.h"
#include <QEvent>
#include <QToolTip>
PushButton::PushButton(QWidget *parent):QPushButton(parent)
{
}

PushButton::~PushButton()
{
}
bool PushButton::event(QEvent *e)
{

  return QPushButton::event(e);
}
void PushButton::setButtonProperty()
{
  setWindowFlags(Qt::FramelessWindowHint);
  setWindowOpacity(0);
}
