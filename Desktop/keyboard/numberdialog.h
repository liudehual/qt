#ifndef NUMBERDIALOG_H
#define NUMBERDIALOG_H

#include <QDialog>

namespace Ui {
  class NumberDialog;
}
class QLineEdit;
class NumberDialog : public QDialog
{
  Q_OBJECT
public:
  void setLineEdit(QLineEdit *edit){m_edit=edit;}
  static NumberDialog *instance();
public slots:
  void pushButtonOneSlot();
  void pushButtonTwoSlot();
  void pushButtonThreeSlot();
  void pushButtonFourSlot();
  void pushButtonFiveSlot();
  void pushButtonSixSlot();
  void pushButtonSevenSlot();
  void pushButtonEightSlot();
  void pushButtonNineSlot();
  void pushButtonZeroSlot();
  void pushButtonBackspaceSlot();
  void pushButtonOkSlot();
  void pushButtonDelSlot();
  void pushButtonPointSlot();
private:
  explicit NumberDialog(QWidget *parent = 0);
  ~NumberDialog();
private:
  void createConnection();
  void setLineEditText(QString);
private:
  QLineEdit *m_edit;
  static NumberDialog *m_instance;
private:
  Ui::NumberDialog *ui;

};

#endif // NUMBERDIALOG_H
