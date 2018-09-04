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
    //bg->setMainWindow(this);
    initEvent();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initEvent()
{
    /* file menu */
    QObject::connect(ui->action_create_new,SIGNAL(triggered()),this,SLOT(myCreateNewFile()));
    QObject::connect(ui->action_open,SIGNAL(triggered()),this,SLOT(myOpenFile()));
    QObject::connect(ui->action_save,SIGNAL(triggered()),this,SLOT(mySaveFile()));
    QObject::connect(ui->action_save_as,SIGNAL(triggered()),this,SLOT(mySaveAsFile()));
    QObject::connect(ui->action_page_set,SIGNAL(triggered()),this,SLOT(myPageSet()));
    QObject::connect(ui->action_print_preview,SIGNAL(triggered()),this,SLOT(myPrintPreview()));

    QObject::connect(ui->action_print,SIGNAL(triggered()),this,SLOT(myPrint()));
    QObject::connect(ui->action_exit,SIGNAL(triggered()),qApp,SLOT(quit()));

    /* edit menu */
    QObject::connect(ui->action_revoke,SIGNAL(triggered()),this,SLOT(myRevoke()));
    QObject::connect(ui->action_cut,SIGNAL(triggered()),this,SLOT(myCut()));
    QObject::connect(ui->action_copy,SIGNAL(triggered()),this,SLOT(myCopy()));
    QObject::connect(ui->action_paste,SIGNAL(triggered()),this,SLOT(myPaste()));
    QObject::connect(ui->action_delete,SIGNAL(triggered()),this,SLOT(myDelete()));
    QObject::connect(ui->action_find,SIGNAL(triggered()),this,SLOT(myFind()));
    QObject::connect(ui->action_find_next,SIGNAL(triggered()),this,SLOT(myFindNext()));
    QObject::connect(ui->action_replace,SIGNAL(triggered()),this,SLOT(myReplace()));
    QObject::connect(ui->action_go_to,SIGNAL(triggered()),this,SLOT(myGoto()));
    QObject::connect(ui->action_select_all,SIGNAL(triggered()),this,SLOT(mySelectAll()));
    QObject::connect(ui->action_date_and_time,SIGNAL(triggered()),this,SLOT(myDatatime()));
    QObject::connect(ui->action_about_file,SIGNAL(triggered()),this,SLOT(myAboutFile()));///=========

    /* format menu */
    QObject::connect(ui->action_auto_new_line,SIGNAL(triggered()),this,SLOT(myAutoWrap()));
    QObject::connect(ui->action_front,SIGNAL(triggered()),this,SLOT(myFront()));

    /* view menu*/
    QObject::connect(ui->action_status,SIGNAL(triggered()),this,SLOT(myStatus()));

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
void MainWindow::myOpenFile()
{

}
void MainWindow::mySaveFile()
{

}
void MainWindow::mySaveAsFile()
{

}
void MainWindow::myCreateNewFile()
{

}
void MainWindow::myPrint()
{

}
void MainWindow::myRevoke()
{

}
void MainWindow::myCut()
{

}
void MainWindow::myCopy()
{

}
void MainWindow::myPaste()
{

}
void MainWindow::myDelete()
{

}
void MainWindow::myFind()
{

}
void MainWindow::myFindNext()
{

}
void MainWindow::myReplace()
{

}
void MainWindow::myGoto()
{

}
void MainWindow::mySelectAll()
{

}
void MainWindow::myDatatime()
{

}
void MainWindow::myAboutFile()
{

}
void MainWindow::myAutoWrap()
{

}
void MainWindow::myFront()
{

}
void MainWindow::myStatus()
{

}
