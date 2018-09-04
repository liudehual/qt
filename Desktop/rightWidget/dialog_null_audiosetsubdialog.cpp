#include "dialog_null_audiosetsubdialog.h"
#include "ui_dialog_null_audiosetsubdialog.h"

dialog_null_audioSetSubDialog::dialog_null_audioSetSubDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_audioSetSubDialog)
{
  ui->setupUi(this);
}

dialog_null_audioSetSubDialog::~dialog_null_audioSetSubDialog()
{
  delete ui;
}
