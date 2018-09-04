#include "background.h"
#include <QMessageBox>
#include "mainwindow.h"
#include <QTextEdit>
#include <QPrintDialog>
#include <QPrinter>
#include <QPainter>
#include <QPrintPreviewDialog>
#include <QPageSetupDialog>
#include <QFile>
#include <QFileDialog>
#include <QString>
#include <QTextStream>
#include <stdio.h>
#include <stdlib.h>
#include <QDateTime>
BackGround::BackGround(QObject *parent) : QObject(parent)
{

}
void  BackGround::setTextEdit(QTextEdit *tEdit)
{
    if(tEdit!=NULL)   edit=tEdit;
}
void BackGround::setMainWindow(MainWindow *min)
{
    if(min!=NULL)   window=min;
}
void BackGround::myOpenFile()
{
    fileName = QFileDialog::getOpenFileName(window, tr("Open File"), QString(), tr("Text Files (*.txt);;ALL Files (*.*)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(window, tr("Error"), tr("Could not open file"));
            return;
        }
        QTextStream in(&file);
        edit->setText(in.readAll());
        file.close();
    }
}
void BackGround::mySaveFile()
{
    fileName = QFileDialog::getSaveFileName(window, tr("Open File"), "", tr("Config Files (*.txt)"));
    if(!fileName.isEmpty()){
        QFile file(fileName);
        if(!file.open(QIODevice::WriteOnly)){
            QMessageBox::critical(window, tr("Error"), tr("Could not open file"));
            return;
        }
        QTextStream out(&file);
        out<<edit->toPlainText();
        file.close();
    }
}
void BackGround::mySaveAsFile()
{

}
void BackGround::myCreateNewFile()
{

}
void BackGround::myPageSet()
{
    QPrinter printer;
    QPageSetupDialog pageSetUpdlg(&printer, window);
    if (pageSetUpdlg.exec() == QDialog::Accepted){
        printer.setOrientation(QPrinter::Landscape);
    }else{
        printer.setOrientation(QPrinter::Portrait);
    }
}

void BackGround::myPrintPreview()
{
      QPrinter printer;
      // 创建打印预览对话框
      QPrintPreviewDialog preview(&printer, window);
      // 当要生成预览页面时，发射paintRequested()信号
      connect(&preview, SIGNAL(paintRequested(QPrinter*)), this,SLOT(print(QPrinter*)));
       preview.exec();
}

void BackGround::myPrint()
{
       // 创建打印机对象
        QPrinter printer;
        QString printerName = printer.printerName();
        if( printerName.size() == 0) return;

        // 创建打印对话框
        QPrintDialog dlg(&printer, window);

        //如果编辑器中有选中区域，则打印选中区域
        if (edit->textCursor().hasSelection())
            dlg.addEnabledOption(QAbstractPrintDialog::PrintSelection);

        // 如果在对话框中按下了打印按钮，则执行打印操作
        if (dlg.exec() == QDialog::Accepted) {
           edit->print(&printer);
        }
}

void  BackGround::print(QPrinter *printer)
{
    edit->print(printer);
}

void BackGround::myRevoke()
{

}
void BackGround::myCut()
{

}
void BackGround::myCopy()
{

}
void BackGround::myPaste()
{

}
void BackGround::myDelete()
{

}
void BackGround::myFind()
{

}
void BackGround::myFindNext()
{

}
void BackGround::myReplace()
{

}
void BackGround::myGoto()
{

}
void BackGround::mySelectAll()
{

}
void BackGround::myDatatime()
{

}
void BackGround::myAboutFile()
{
    if(fileName.isEmpty()){
        return;
    }
    QFileInfo info(fileName);
    qint64 size=info.size();
    QDateTime createTime=info.created();
    QDateTime lastModifyTime=info.lastModified();
    QDateTime lastReadTime=info.lastRead();
   QString myFileName=info.fileName();
#if 0
    bool isDir=info.isDir();
    bool isFile=info.isFile();
    bool isSymLink = info.isSymLink();
    bool isHidden = info.isHidden();
    bool isReadable = info.isReadable();
    bool isWritable = info.isWritable();
    bool isExecutable =info.isExecutable();
#endif
    char *format=" FileName:  %s\n\r"
                            "File SIze:  %d\n\r"
                            "Create Time:  %s\n\r"
                            "Last Modified Time:  %s\n\r";


    char fileInfo[2*1024];
    sprintf(fileInfo,format,myFileName.toStdString().c_str(),size,createTime.toString().toStdString().c_str(),lastModifyTime.toString().toStdString().c_str());
    QMessageBox::information(window, tr("About File"),
            tr(fileInfo), QMessageBox::Yes | QMessageBox::No);
}
void BackGround::myAutoWrap()
{

}
void BackGround::myFront()
{

}
void BackGround::myStatus()
{

}
