#ifndef LABEL_H
#define LABEL_H
#include <QLabel>

class Label : public QLabel
{
  Q_OBJECT
public:
 explicit Label(QWidget *parent=0);
  virtual ~Label();
protected:
  bool event(QEvent *);
};

#endif // LABEL_H
