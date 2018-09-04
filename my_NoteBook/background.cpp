#include "background.h"
#include <QMessageBox>
#include "mainwindow.h"
#include <QTextEdit>
BackGround::BackGround(QObject *parent) : QObject(parent)
{

}
void  BackGround::setTextEdit(QTextEdit *tEdit)
{
    if(tEdit!=NULL)edit=tEdit;
}
void BackGround::myOpenFile()
{

}
void BackGround::mySaveFile()
{

}
void BackGround::mySaveAsFile()
{

}
void BackGround::myCreateNewFile()
{

}
void BackGround::myPrint()
{

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


