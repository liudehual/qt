#include "previewdialog.h"
#include "ui_previewsetdialog.h"

dialog_null_PreviewSetItemDialog::dialog_null_PreviewSetItemDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_PreviewSetItemDialog)
{
  ui->setupUi(this);
}

dialog_null_PreviewSetItemDialog::~dialog_null_PreviewSetItemDialog()
{
  delete ui;
}
