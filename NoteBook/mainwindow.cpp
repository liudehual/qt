#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "background.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    bg=new BackGround();
    bg->setTextEdit(ui->textEdit);
    bg->setMainWindow(this);
    initEvent();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initEvent()
{
    /* file menu */
    QObject::connect(ui->action_create_new,SIGNAL(triggered()),bg,SLOT(myCreateNewFile()));
    QObject::connect(ui->action_open,SIGNAL(triggered()),bg,SLOT(myOpenFile()));
    QObject::connect(ui->action_save,SIGNAL(triggered()),bg,SLOT(mySaveFile()));
    QObject::connect(ui->action_save_as,SIGNAL(triggered()),bg,SLOT(mySaveAsFile()));
    QObject::connect(ui->action_page_set,SIGNAL(triggered()),bg,SLOT(myPageSet()));
    QObject::connect(ui->action_print_preview,SIGNAL(triggered()),bg,SLOT(myPrintPreview()));
    QObject::connect(ui->action_print,SIGNAL(triggered()),bg,SLOT(myPrint()));
    QObject::connect(ui->action_exit,SIGNAL(triggered()),qApp,SLOT(quit()));

    /* edit menu */
    QObject::connect(ui->action_revoke,SIGNAL(triggered()),bg,SLOT(myRevoke()));
    QObject::connect(ui->action_cut,SIGNAL(triggered()),bg,SLOT(myCut()));
    QObject::connect(ui->action_copy,SIGNAL(triggered()),bg,SLOT(myCopy()));
    QObject::connect(ui->action_paste,SIGNAL(triggered()),bg,SLOT(myPaste()));
    QObject::connect(ui->action_delete,SIGNAL(triggered()),bg,SLOT(myDelete()));
    QObject::connect(ui->action_find,SIGNAL(triggered()),bg,SLOT(myFind()));
    QObject::connect(ui->action_find_next,SIGNAL(triggered()),bg,SLOT(myFindNext()));
    QObject::connect(ui->action_replace,SIGNAL(triggered()),bg,SLOT(myReplace()));
    QObject::connect(ui->action_go_to,SIGNAL(triggered()),bg,SLOT(myGoto()));
    QObject::connect(ui->action_select_all,SIGNAL(triggered()),bg,SLOT(mySelectAll()));
    QObject::connect(ui->action_date_and_time,SIGNAL(triggered()),bg,SLOT(myDatatime()));
    QObject::connect(ui->action_about_file,SIGNAL(triggered()),bg,SLOT(myAboutFile()));///=========

    /* format menu */
    QObject::connect(ui->action_auto_new_line,SIGNAL(triggered()),bg,SLOT(myAutoWrap()));
    QObject::connect(ui->action_front,SIGNAL(triggered()),bg,SLOT(myFront()));

    /* view menu*/
    QObject::connect(ui->action_status,SIGNAL(triggered()),bg,SLOT(myStatus()));

    /* about menu */
    QObject::connect(ui->action_about,SIGNAL(triggered()),this,SLOT(about()));
    QObject::connect(ui->action_QT,SIGNAL(triggered()),qApp,SLOT(aboutQt()));///=========
}

void MainWindow::about()
{
        QMessageBox::about(this, tr("NoteBook"),
                tr("<p>程序名：<b>NoteBook</b></p> "
                   "<p>日期: 2015-09-30</p>"
                   "<p>作者： GCT</p>"
                   " <p> Copyright (C) 2015 The GCT Ltd.</p>"
                   ));
}

