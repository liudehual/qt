#include "dialog_null_videolostsetsubdialog.h"
#include "ui_dialog_null_videolostsetsubdialog.h"

dialog_null_videoLostSetSubDialog::dialog_null_videoLostSetSubDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_videoLostSetSubDialog)
{
  ui->setupUi(this);
}

dialog_null_videoLostSetSubDialog::~dialog_null_videoLostSetSubDialog()
{
  delete ui;
}
