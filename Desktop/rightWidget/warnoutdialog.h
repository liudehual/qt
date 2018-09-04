#ifndef WARNOUTDIALOG_H
#define WARNOUTDIALOG_H

#include <QDialog>

namespace Ui {
  class dialog_null_warnOutDialog;
}

class dialog_null_warnOutDialog : public QDialog
{
  Q_OBJECT

public:
  explicit dialog_null_warnOutDialog(QWidget *parent = 0);
  ~dialog_null_warnOutDialog();

public slots:
 void checkBoxWarnOutDialogSendToOther(int);
 void checkBoxWarnOutDialogEmailWarn(int);
 void checkBoxWarnOutDialogOpenVideo(int);
 void checkBoxWarnOutDialogSendPic(int);
 void checkBoxWarnOutDialogCapturePic(int);
 void checkBoxWarnOutDialogCallPreset(int);
 void checkBoxWarnOutDialogOpenWarnOut(int);
 void checkBoxWarnOutDialogWarn(int);
 void comboBoxWarnOutDialogSendToOther(int);
 void comboBoxWarnOutDialogSendPic(int);
 void comboBoxWarnOutDialogOpenVideo(int);
 void comboBoxWarnOutDialogCallPreset(int);
 void comboBoxWarnOutDialogCapturePic(int);
 void comboBoxWarnOutDialogId(int);
 void pushButtonWarnOutDialogUse();
 void pushButtonWarnOutDialogSure();
 void pushButtonWarnOutDialogCancel();
protected:
 bool eventFilter(QObject *, QEvent *);
private:
  Ui::dialog_null_warnOutDialog *ui;
};

#endif // dialog_null_warnOutDialog_H
