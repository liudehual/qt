#include "dialog_null_yuntaicruisedialog.h"
#include "ui_dialog_null_yuntaicruisedialog.h"

dialog_null_yunTaiCruiseDialog::dialog_null_yunTaiCruiseDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_yunTaiCruiseDialog)
{
  ui->setupUi(this);
}

dialog_null_yunTaiCruiseDialog::~dialog_null_yunTaiCruiseDialog()
{
  delete ui;
}
