#include "dialog_null_yuntaipresetdialog.h"
#include "ui_dialog_null_yuntaipresetdialog.h"

dialog_null_yunTaiPresetDialog::dialog_null_yunTaiPresetDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_yunTaiPresetDialog)
{
  ui->setupUi(this);
}

dialog_null_yunTaiPresetDialog::~dialog_null_yunTaiPresetDialog()
{
  delete ui;
}
