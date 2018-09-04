#include "yuntaidialog.h"
#include "ui_yuntaidialog.h"
#include "yuntaisubdialog.h"
#include <QMouseEvent>
#include <QFile>
#include "tools/proxy.h"

#define DEBUG 1
#if (DEBUG==0 || DEBUG==1)
#include <stdio.h>
#include <stdlib.h>
#endif
#if (DEBUG==0)
#define debug(fmt,...) fprintf(stderr," "fmt"File: %s Function:%s LineNumber:%d",__VA_ARGS__,__FILE__,__FUNCTION__,__LINE__)
#elif (DEBUG==1)
#define debug(fmt,...) fprintf(stderr,fmt,__VA_ARGS__)
#elif (DEBUG==2)
#define debug(fmt,...)
#endif

dialog_null_yunTaiDialog::dialog_null_yunTaiDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialog_null_yunTaiDialog)
{
    ui->setupUi(this);
    subDialogShowing=false;

    QObject::connect(Proxy::instance(),SIGNAL(sendMessageToYunTaiDialog(QString)),this,SLOT(recvMessageFromProxy(QString)));
   QObject::connect(this,SIGNAL(sendMessageToProxy(QString)),Proxy::instance(),SLOT(recvMessageFromSecondDialog(QString)));

    aperture=0;
    zoomed=0;
    zooming=0;

    ui->horizontalSlider_yunTaiDialog_speed->setMinimum(0);
    ui->horizontalSlider_yunTaiDialog_speed->setMaximum(100);
    initWidget();
    //this->setStyleSheet("QLabel{color:#FFFF00;}QPushButton{color:#FFFFFF;}");
    QObject::connect(ui->pushButton_yunTaiDialog_center,SIGNAL(clicked()),this,SLOT(pushButtonYunTaiDialogCenter()));
    QObject::connect(ui->pushButton_yunTaiDialog_down,SIGNAL(clicked()),this,SLOT(pushButtonYunTaiDialogDown()));
    QObject::connect(ui->pushButton_yunTaiDialog_right,SIGNAL(clicked()),this,SLOT(pushButtonYunTaiDialogRight()));
    QObject::connect(ui->pushButton_yunTaiDialog_left,SIGNAL(clicked()),this,SLOT(pushButtonYunTaiDialogLeft()));
    QObject::connect(ui->pushButton_yunTaiDialog_up,SIGNAL(clicked()),this,SLOT(pushButtonYunTaiDialogUp()));
    QObject::connect(ui->pushButton_yunTaiDialog_apertureSub,SIGNAL(clicked()),this,SLOT(pushButtonYunTaiDialogApertureSub()));
    QObject::connect(ui->pushButton_yunTaiDialog_apertureAdd,SIGNAL(clicked()),this,SLOT(pushButtonYunTaiDialogApertureAdd()));
    QObject::connect(ui->pushButton_yunTaiDialog_zoomedAdd,SIGNAL(clicked()),this,SLOT(pushButtonYunTaiDialogZoomedAdd()));
    QObject::connect(ui->pushButton_yunTaiDialog_zoomedSub,SIGNAL(clicked()),this,SLOT(pushButtonYunTaiDialogZoomedSub()));
    QObject::connect(ui->pushButton_yunTaiDialog_zoomingAdd,SIGNAL(clicked()),this,SLOT(pushButtonYunTaiDialogZoomingAdd()));
    QObject::connect(ui->pushButton_yunTaiDialog_zoomingSub,SIGNAL(clicked()),this,SLOT(pushButtonYunTaiDialogZoomingSub()));
    QObject::connect(ui->pushButton_yunTaiDialog_more,SIGNAL(clicked()),this,SLOT(pushButtonYunTaiDialogMore()));
    QObject::connect(ui->comboBox_yunTaiDialog_channel,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxYunTaiDialogChannel(int)));

    loadQssFile("qss/yuntai.qss");
}

dialog_null_yunTaiDialog::~dialog_null_yunTaiDialog()
{
    delete ui;
}
void dialog_null_yunTaiDialog::recvMessageFromProxy(QString message)
{

}
void dialog_null_yunTaiDialog::myHide()
{
    this->hide();
    dialog_null_yunTaiSubDialog*subDialog= (dialog_null_yunTaiSubDialog *)(*subMap.find("dialog_null_yunTaiSubDialog"));
    subDialog->hide();
}
bool dialog_null_yunTaiDialog::eventFilter(QObject *object, QEvent *e)
{
    if(object->objectName()=="label_yunTaiDialog_shutdown"){
        if(e->type()==QEvent::MouseButtonPress){
            QMouseEvent *mouse=dynamic_cast<QMouseEvent *>(e);
            if(mouse->button()==Qt::LeftButton){
                if(subDialogShowing){
                    ui->pushButton_yunTaiDialog_more->setText(QString::fromUtf8("更多"));
                    subDialogShowing=false;
                    dialog_null_yunTaiSubDialog*subDialog= (dialog_null_yunTaiSubDialog *)(*subMap.find("dialog_null_yunTaiSubDialog"));
                    subDialog->hide();
                }
                this->hide();
            }
        }
    }
    return QDialog::eventFilter(object,e);
}
void dialog_null_yunTaiDialog::initWidget()
{
    //  ui->label_yunTaiDialog_shutdown->setObjectName("shutdown");
    ui->label_yunTaiDialog_shutdown->installEventFilter(this);

    subMap.clear();
    dialog_null_yunTaiSubDialog *dialog=new dialog_null_yunTaiSubDialog(this);
    dialog->setObjectName("dialog_null_yunTaiSubDialog");
    subMap.insert("dialog_null_yunTaiSubDialog",dialog);

    QObject::connect(ui->pushButton_yunTaiDialog_more,SIGNAL(clicked()),this,SLOT(showSubDialog()));
    QObject::connect(ui->horizontalSlider_yunTaiDialog_speed,SIGNAL(valueChanged(int)),this,SLOT(setSpeed(int)));
}
void  dialog_null_yunTaiDialog::setSpeed(int speed)
{
    QString message;
    message.clear();
    QString xmlHead="<Request><YunTaiControl method=\"Set\">";
    message.append(xmlHead);
    message.append("<Rate>"+QString::number(speed)+"</Rate>");
    QString xmlEnd="</YunTaiControl></Request>";
    message.append(xmlEnd);
    emit sendMessageToProxy(message);
}
void dialog_null_yunTaiDialog::showSubDialog()
{
    dialog_null_yunTaiSubDialog*subDialog= (dialog_null_yunTaiSubDialog *)(*subMap.find("dialog_null_yunTaiSubDialog"));

    if(subDialogShowing){
        ui->pushButton_yunTaiDialog_more->setText(QString::fromUtf8("更多"));
        subDialog->hide();
        subDialogShowing=false;
    }else{
        ui->pushButton_yunTaiDialog_more->setText(QString::fromUtf8("隐藏"));
        QRect rect= this->geometry();
        int subX=rect.x();
        int subY=rect.y()+rect.height();
        subDialog->move(subX,subY);
        subDialog->show();
        subDialogShowing=true;
    }
}

void dialog_null_yunTaiDialog::pushButtonYunTaiDialogCenter()
{
    QString message;
    message.clear();
    QString xmlHead="<Request><YunTaiControl method=\"Set\">";
    message.append(xmlHead);
    message.append("<Center>"+QString::number(1)+"</Center>");
    QString xmlEnd="</YunTaiControl></Request>";
    message.append(xmlEnd);
    emit sendMessageToProxy(message);
}

void dialog_null_yunTaiDialog::pushButtonYunTaiDialogDown()
{
    QString message;
    message.clear();
    QString xmlHead="<Request><YunTaiControl method=\"Set\">";
    message.append(xmlHead);
    message.append("<Down>"+QString::number(1)+"</Down>");
    QString xmlEnd="</YunTaiControl></Request>";
    message.append(xmlEnd);
    emit sendMessageToProxy(message);
}

void dialog_null_yunTaiDialog::pushButtonYunTaiDialogRight()
{
    QString message;
    message.clear();
    QString xmlHead="<Request><YunTaiControl method=\"Set\">";
    message.append(xmlHead);
    message.append("<Right>"+QString::number(1)+"</Right>");
    QString xmlEnd="</YunTaiControl></Request>";
    message.append(xmlEnd);
    emit sendMessageToProxy(message);
}

void dialog_null_yunTaiDialog::pushButtonYunTaiDialogLeft()
{
    QString message;
    message.clear();
    QString xmlHead="<Request><YunTaiControl method=\"Set\">";
    message.append(xmlHead);
    message.append("<Left>"+QString::number(1)+"</Left>");
    QString xmlEnd="</YunTaiControl></Request>";
    message.append(xmlEnd);
    emit sendMessageToProxy(message);
}

void dialog_null_yunTaiDialog::pushButtonYunTaiDialogUp()
{
    QString message;
    message.clear();
    QString xmlHead="<Request><YunTaiControl method=\"Set\">";
    message.append(xmlHead);
    message.append("<Up>"+QString::number(1)+"</Up>");
    QString xmlEnd="</YunTaiControl></Request>";
    message.append(xmlEnd);
    emit sendMessageToProxy(message);
}

void dialog_null_yunTaiDialog::pushButtonYunTaiDialogApertureSub()
{
    QString message;
    message.clear();
    QString xmlHead="<Request><YunTaiControl method=\"Set\">";
    message.append(xmlHead);
    if(aperture>0){
        --aperture;
    }else{
        aperture=1;
    }
    message.append("<SubAperture>"+QString::number(aperture)+"</SubAperture>");
    QString xmlEnd="</YunTaiControl></Request>";
    message.append(xmlEnd);
    emit sendMessageToProxy(message);
}

void dialog_null_yunTaiDialog::pushButtonYunTaiDialogApertureAdd()
{
    QString message;
    message.clear();
    QString xmlHead="<Request><YunTaiControl method=\"Set\">";
    message.append(xmlHead);
    message.append("<AddAperture>"+QString::number(++aperture)+"</AddAperture>");
    QString xmlEnd="</YunTaiControl></Request>";
    message.append(xmlEnd);
    emit sendMessageToProxy(message);
}

void dialog_null_yunTaiDialog::pushButtonYunTaiDialogZoomedAdd()
{
    QString message;
    message.clear();
    QString xmlHead="<Request><YunTaiControl method=\"Set\">";
    message.append(xmlHead);
    message.append("<AddZoom>"+QString::number(++zoomed)+"</AddZoom>");
    QString xmlEnd="</YunTaiControl></Request>";
    message.append(xmlEnd);
    emit sendMessageToProxy(message);
}

void dialog_null_yunTaiDialog::pushButtonYunTaiDialogZoomedSub()
{
    QString message;
    message.clear();
    QString xmlHead="<Request><YunTaiControl method=\"Set\">";
    message.append(xmlHead);
    if(zoomed>0){
        --zoomed;
    }else{
        zoomed=0;
    }
    message.append("<SubZoom>"+QString::number(zoomed)+"</SubZoom>");
    QString xmlEnd="</YunTaiControl></Request>";
    message.append(xmlEnd);
    emit sendMessageToProxy(message);
}

void dialog_null_yunTaiDialog::pushButtonYunTaiDialogZoomingAdd()
{
    QString message;
    message.clear();
    QString xmlHead="<Request><YunTaiControl method=\"Set\">";
    message.append(xmlHead);

    message.append("<AddZoomed>"+QString::number(++zooming)+"</AddZoomed>");
    QString xmlEnd="</YunTaiControl></Request>";
    message.append(xmlEnd);
    emit sendMessageToProxy(message);
}

void dialog_null_yunTaiDialog::pushButtonYunTaiDialogZoomingSub()
{
    QString message;
    message.clear();
    QString xmlHead="<Request><YunTaiControl method=\"Set\">";
    message.append(xmlHead);

    if(zooming>0){
        --zooming;
    }else{
        zooming=0;
    }
    message.append("<SubZoomed>"+QString::number(zooming)+"</SubZoomed>");
    QString xmlEnd="</YunTaiControl></Request>";
    message.append(xmlEnd);
    emit sendMessageToProxy(message);
}

void dialog_null_yunTaiDialog::pushButtonYunTaiDialogMore()
{

}

void dialog_null_yunTaiDialog::comboBoxYunTaiDialogChannel(int index)
{
    QString message;
    message.clear();
    QString xmlHead="<Request><YunTaiControl method=\”Set\”>";
    message.append(xmlHead);
    message.append("<ChannelId>"+ui->comboBox_yunTaiDialog_channel->currentText()+"</ChannelId>");
    QString xmlEnd="</YunTaiControl></Request>";
    message.append(xmlEnd);
    emit sendMessageToProxy(message);
}
void dialog_null_yunTaiDialog::loadQssFile(QString fileName)
{
    QFile qssFile(fileName);
    QString qss;
    qssFile.open(QFile::ReadOnly);
    if(qssFile.isOpen()){
        qss = QLatin1String(qssFile.readAll());
        this->setStyleSheet(qss);
        qssFile.close();
    }
}
