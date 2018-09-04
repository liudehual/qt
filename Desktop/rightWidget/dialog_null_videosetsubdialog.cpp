#include "dialog_null_videosetsubdialog.h"
#include "ui_dialog_null_videosetsubdialog.h"

dialog_null_videoSetSubDialog::dialog_null_videoSetSubDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_videoSetSubDialog)
{
  ui->setupUi(this);
}

dialog_null_videoSetSubDialog::~dialog_null_videoSetSubDialog()
{
  delete ui;
}
