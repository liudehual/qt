#ifndef PICLAYOUTDIALOG_H
#define PICLAYOUTDIALOG_H

#include <QDialog>

namespace Ui {
  class dialog_null_picLayoutDialog;
}
class QComboBox;
class dialog_null_picLayoutDialog : public QDialog
{
  Q_OBJECT

public:
  explicit dialog_null_picLayoutDialog(QWidget *parent = 0);
  ~dialog_null_picLayoutDialog();
  void createFunctionAndConnection(QString objectName);

  void testFile();
  void setComboValue(QString,QString,QComboBox *);
private:
  void parseXml(QString);
signals:
  void sendMessageToProxy(QString);
public slots:
  void recvMessageFromProxy(QString);
public slots:
  void pushButtonPicLayoutDialogPicOne();
  void pushButtonPicLayoutDialogPicTwo();
  void pushButtonPicLayoutDialogPicThree();
  void pushButtonPicLayoutDialogPicFour();
  void pushButtonPicLayoutDialogPicFive();
  void pushButtonPicLayoutDialogPicSix();
  void pushButtonPicLayoutDialogPicSeven();
  void comboBoxPicLayoutDialogPicExchange(int);
  void pushButtonPicLayoutDialogPicEight();
  void comboBoxPicLayoutDialogDataShow(int);
  void pushButtonPicLayoutDialogMulPicEight();
  void pushButtonPicLayoutDialogMulPicFour();
  void pushButtonPicLayoutDialogMulPicNine();
  void pushButtonPicLayoutDialogMulPicSixteen();
  void pushButtonPicLayouDialogUse();
  void pushButtonPicLayouDialogSure();
  void pushButtonPicLayouDialogCancel();

protected:
  bool eventFilter(QObject *, QEvent *);
private:
  void initWidget();
  void loadQssFile(QString fileName);

private:
  Ui::dialog_null_picLayoutDialog *ui;
  QString messageContent;
  bool mulPic;
  int pic;
  bool ifUse;
};

#endif // dialog_null_picLayoutDialog_H
