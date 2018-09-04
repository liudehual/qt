#ifndef PLAYBACKDIALOG_H
#define PLAYBACKDIALOG_H

#include <QDialog>

namespace Ui {
  class dialog_null_playbackDialog;
}
class QComboBox;
class dialog_null_playbackDialog : public QDialog
{
  Q_OBJECT

public:
  explicit dialog_null_playbackDialog(QWidget *parent = 0);
  ~dialog_null_playbackDialog();
  void createFunctionAndConnection(QString objectName);
  void testFile();
private:
  void parseXml(QString);
  void createMessage();
signals:
  void sendMessageToProxy(QString);
public slots:
  void recvMessageFromProxy(QString);
  void sliderValueChanged(int);
private:
  void setComboValue(QString ,QString ,QComboBox *);

public slots:
  void pushButtonPlaybackDialogBack();
  void pushButtonPlaybackDialogPlay();
  void pushButtonPlaybackDialogForward();
  void pushButtonPlaybackDialogSpeedUp();
  void comboBoxPlaybackDialogSelectChannel(int);
  void pushButtonPlaybackDialogHide();
protected:
  bool eventFilter(QObject *, QEvent *);
private:
  void loadQssFile(QString fileName);

private:
  void initWidget();
private:
  Ui::dialog_null_playbackDialog *ui;

  int startPlay;
  int backPlay;
  int forwardPlay;

};

#endif // dialog_null_playbackDialog_H
