#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QPushButton>
class PushButton : public QPushButton
{
  Q_OBJECT
public:
  explicit PushButton(QWidget *parent=0);
  virtual ~PushButton();
  void setButtonProperty();
public slots:

protected:
  bool event(QEvent *);
};

#endif // PUSHBUTTON_H
