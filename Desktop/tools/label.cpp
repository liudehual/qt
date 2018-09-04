#include "label.h"

Label::Label(QWidget *parent):QLabel(parent)
{
}
Label::~Label()
{
}
bool Label::event(QEvent *e)
{
  return QLabel::event(e);
}
