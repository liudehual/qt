#include "combobox.h"

ComboBox::ComboBox(QWidget *parent):QComboBox(parent)
{

}

ComboBox::~ComboBox()
{

}
bool ComboBox::event(QEvent *e)
{

  return QComboBox::event(e);
}
