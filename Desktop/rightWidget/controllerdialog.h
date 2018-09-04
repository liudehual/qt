#ifndef dialog_null_controllerDialog_H
#define dialog_null_controllerDialog_H

#include <QDialog>
#include <QMap>
namespace Ui {
  class dialog_null_controllerDialog;
}
class QComboBox;
class dialog_null_controllerDialog : public QDialog
{
  Q_OBJECT

public:
  explicit dialog_null_controllerDialog(QWidget *parent = 0);
  ~dialog_null_controllerDialog();
  void createFunctionAndConnection(QString objectName);
  void testFile();
  void startXml(QString);
  void setComboValue(QString,QString,QComboBox *);

signals:
  void sendMessageToProxy(QString);
public slots:
  void recvMessageFromProxy(QString);

  void pushButtonControllerDialogSure();
  void pushButtonControllerDialogCancel();
  void pushButtonControllerDialogUse();

  void comboBoxControllerDialogControllerOne(int);
  void comboBoxControllerDialogControllerTwo(int);
  void comboBoxControllerDialogControllerThree(int);
  void comboBoxControllerDialogControllerFour(int);

protected:
  bool eventFilter(QObject *, QEvent *);

  QMap<QString,QDialog *> childMap;
private:
  void initWidget();
  void loadQssFile(QString fileName);


private:
  Ui::dialog_null_controllerDialog *ui;
  QMap<QString,QObject *> widgetMap;
};

#endif // dialog_null_controllerDialog_H
