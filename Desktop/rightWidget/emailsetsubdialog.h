#ifndef DIALOG_NULL_EMAILSETSUBDIALOG_H
#define DIALOG_NULL_EMAILSETSUBDIALOG_H

#include <QDialog>
class QLabel;
namespace Ui {
  class dialog_null_emailSetSubDialog;
}

class dialog_null_emailSetSubDialog : public QDialog
{
  Q_OBJECT

public:
  explicit dialog_null_emailSetSubDialog(QWidget *parent = 0);
  ~dialog_null_emailSetSubDialog();
QLabel * getLabe();
private:
  Ui::dialog_null_emailSetSubDialog *ui;
};

#endif // DIALOG_NULL_EMAILSETSUBDIALOG_H
