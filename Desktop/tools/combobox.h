#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <QComboBox>
class ComboBox : public QComboBox
{
  Q_OBJECT
public:
 explicit ComboBox(QWidget *parent=0);
  virtual ~ComboBox();
protected:
  bool event(QEvent *);
};

#endif // COMBOBOX_H
