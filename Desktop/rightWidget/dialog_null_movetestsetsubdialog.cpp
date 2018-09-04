#include "dialog_null_movetestsetsubdialog.h"
#include "ui_dialog_null_movetestsetsubdialog.h"

dialog_null_moveTestSetSubDialog::dialog_null_moveTestSetSubDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_moveTestSetSubDialog)
{
  ui->setupUi(this);
}

dialog_null_moveTestSetSubDialog::~dialog_null_moveTestSetSubDialog()
{
  delete ui;
}
