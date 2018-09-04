#include "lineedit.h"

LineEdit::LineEdit(QWidget *parent):QLineEdit(parent)
{

}
LineEdit::~LineEdit()
{

}
bool LineEdit::event(QEvent *e)
{

  return QLineEdit::event(e);
}
