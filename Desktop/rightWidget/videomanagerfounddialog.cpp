﻿#include "videomanagerfounddialog.h"
#include "ui_videomanagerfounddialog.h"

dialog_null_videoManagerFoundDialog::dialog_null_videoManagerFoundDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_videoManagerFoundDialog)
{
  ui->setupUi(this);
}

dialog_null_videoManagerFoundDialog::~dialog_null_videoManagerFoundDialog()
{
  delete ui;
}
