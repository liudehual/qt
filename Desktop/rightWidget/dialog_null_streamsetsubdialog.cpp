#include "dialog_null_streamsetsubdialog.h"
#include "ui_dialog_null_streamsetsubdialog.h"

dialog_null_streamSetSubDialog::dialog_null_streamSetSubDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_streamSetSubDialog)
{
  ui->setupUi(this);
}

dialog_null_streamSetSubDialog::~dialog_null_streamSetSubDialog()
{
  delete ui;
}
