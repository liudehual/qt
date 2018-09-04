#ifndef LINEEDIT_H
#define LINEEDIT_H
#include <QLineEdit>

class LineEdit : public QLineEdit
{
  Q_OBJECT
public:
 explicit LineEdit(QWidget *parent=0);
  virtual ~LineEdit();
protected:
  bool event(QEvent *);
};

#endif // LINEEDIT_H
