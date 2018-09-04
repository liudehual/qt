#include "systemSetDialog.h"
#include "ui_systemSetDialog.h"
#include <QMouseEvent>
#include "tools/connector.h"
#include <QFile>
#include <QTextStream>
#include "keyboard/softkeyboard.h"
#include "tipdialog.h"
#include "tools/proxy.h"
#include <QDesktopWidget>
#include <QDomDocument>
#include "tools/proxy.h"
#include "edituserdialog.h"
#include "tipdialog.h"
#include "emailsetsubdialog.h"
#include "picmanagersubdialog.h"
#include "logsetsubdialog.h"
#include "memorydevicesetsubdialog.h"

#define DEBUG 1

#if (DEBUG==0 || DEBUG==1)
#include <stdio.h>
#include <stdlib.h>
#endif
#if (DEBUG==0)
#define debug(fmt,...) fprintf(stderr," "fmt"Function:%s",__VA_ARGS__,__FUNCTION__,)
#elif (DEBUG==1)
#define debug(fmt,...) fprintf(stderr,fmt,__VA_ARGS__)
#elif (DEBUG==2)
#define debug(fmt,...)
#endif

dialog_null_systemSetDialog::dialog_null_systemSetDialog(QWidget *parent):
  QDialog(parent),
  ui(new Ui::dialog_null_systemSetDialog)
{

  ui->setupUi(this);
  this->setWindowFlags(Qt::FramelessWindowHint );


  p=Proxy::instance();
  QObject::connect(p,SIGNAL(sendMessageToSystemSetDialog(QString)),this,SLOT(recvMessageFromProxy(QString)));
  QObject::connect(this,SIGNAL(sendMessageToProxy(QString)),p,SLOT(recvMessageFromSecondDialog(QString)));

  editUser=new dialog_null_editUserDialog();

  oldMenu.clear();
  widgetMap.clear();
  userName.clear();
  oldName.clear();
  tDialog= new dialog_null_TipDialog;
  initWidget();
  testFile();
  use=false;
}

dialog_null_systemSetDialog::~dialog_null_systemSetDialog()
{
  delete ui;
}
bool dialog_null_systemSetDialog::eventFilter(QObject *object, QEvent *e)
{
  if(object->objectName()=="label_systemSetDialog_shutdown"){
      if(e->type()==QEvent::MouseButtonPress){
          QMouseEvent *mouse=dynamic_cast<QMouseEvent *>(e);
          if(mouse->button()==Qt::LeftButton){
              this->hide();
            }
        }
    }
  if(e->type()==QEvent::MouseButtonPress){
      if(object->objectName()== "lineEdit_log_selectAll" ||
         object->objectName()== "lineEdit_log_date" ||
         object->objectName()== "lineEdit_weihu_restart" ||
         object->objectName()== "lineEdit_picManager_open" ||
         object->objectName()== "lineEdit_picManager_end" ||
         object->objectName()== "lineEdit_email_serverPort" ||
         object->objectName()== "lineEdit_email_userName" ||
         object->objectName()== "lineEdit_email_userpassword" ||
         object->objectName()== "lineEdit_email_interval" ||
         object->objectName()== "lineEdit_wireless_ip" ||
         object->objectName()== "lineEdit_wireless_gateway" ||
         object->objectName()== "lineEdit_wireless_dns" ||
         object->objectName()== "lineEdit_wireless_mask" ||
         object->objectName()== "lineEdit_wireless_mac" ||
         object->objectName()== "lineEdit_wifi_ADSLName" ||
         object->objectName()== "lineEdit_wifi_ADSLPassword" ||
         object->objectName()== "lineEdit_wifi_cardPassword" ||
         object->objectName()== "lineEdit_wifi_cardName" ||
         object->objectName()== "lineEdit_service_videoForwardingAddr" ||
         object->objectName()== "lineEdit_service_videoForwardingPort" ||
         object->objectName()== "lineEdit_service_videoUploadPort" ||
         object->objectName()== "lineEdit_service_videoUploadAddr" ||
         object->objectName()== "lineEdit_stream_minorRate_1" ||
         object->objectName()== "lineEdit_stream_mainRate_1" ||
         object->objectName()== "lineEdit_stream_mainRate_2" ||
         object->objectName()== "lineEdit_stream_minorRate_2" ||
         object->objectName()== "lineEdit_stream_mainRate_3" ||
         object->objectName()== "lineEdit_stream_mainRate_4" ||
         object->objectName()== "lineEdit_stream_minorRate_3" ||
         object->objectName()== "lineEdit_stream_minorRate_4" ||
         object->objectName()== "lineEdit_stream_mainRate_5" ||
         object->objectName()== "lineEdit_stream_mainRate_6" ||
         object->objectName()== "lineEdit_stream_minorRate_5" ||
         object->objectName()== "lineEdit_stream_minorRate_6" ||
         object->objectName()== "lineEdit_stream_mainRate_7" ||
         object->objectName()== "lineEdit_stream_mainRate_8" ||
         object->objectName()== "lineEdit_stream_minorRate_7" ||
         object->objectName()== "lineEdit_stream_minorRate_8" ||
         object->objectName()== "lineEdit_preview_channelName_1" ||
         object->objectName()== "lineEdit_preview_channelName_2" ||
         object->objectName()== "lineEdit_preview_channelName_3" ||
         object->objectName()== "lineEdit_preview_channelName_4" ||
         object->objectName()== "lineEdit_preview_channelName_5" ||
         object->objectName()== "lineEdit_preview_channelName_6" ||
         object->objectName()== "lineEdit_preview_channelName_7" ||
         object->objectName()== "lineEdit_preview_channelName_8" ||
         object->objectName()== "lineEdit_audioSet_listen_1" ||
         object->objectName()== "lineEdit_audioSet_listen_2" ||
         object->objectName()== "lineEdit_audioSet_listen_3" ||
         object->objectName()== "lineEdit_audioSet_listen_4" ||
         object->objectName()== "lineEdit_user_autoLoginOut" ||
         object->objectName()== "lineEdit_time_dateSet" ||
         object->objectName()== "lineEdit_time_timeSet"){
          //debug("%s\n","=============");
          QLineEdit *lEdit=dynamic_cast<QLineEdit *>(object);
          int width=QApplication::desktop()->width();
          int height=QApplication::desktop()->height();
          QPoint point=QCursor::pos();;
          int x=point.x();
          int y=point.y()+20;
          SoftKeyboard *sk=SoftKeyboard::instance();
          sk->hide();
          if(width-x>=sk->width()){
              sk->move(x-20,y);
            }else{
              sk->move(width-sk->width()-20,y);
            }

          sk->show();
          sk->setLineText(lEdit);
        }else{
          debug("%s\n","===============++++++++++++++");
          SoftKeyboard *sk=SoftKeyboard::instance();
          sk->hide();
        }
    }

  return QDialog::eventFilter(object,e);
}
void dialog_null_systemSetDialog::initWidget()
{
  ui->label_systemSetDialog_shutdown->setText("  X");
  ui->label_systemSetDialog_shutdown->installEventFilter(this);

  Connector *con1=new Connector(ui->stackedWidget_general,this);
  QObject::connect(ui->pushButton_general_subGeneral,SIGNAL(clicked()),con1,SLOT(currentIndex0()));
  // debug("<!---- %s ---->\n",ui->pushButton_general->objectName().toStdString().c_str());
  QObject::connect(ui->pushButton_general_time,SIGNAL(clicked()),con1,SLOT(currentIndex1()));
  QObject::connect(ui->pushButton_general_user,SIGNAL(clicked()),con1,SLOT(currentIndex2()));

  QObject::connect(ui->pushButton_general_subGeneral,SIGNAL(clicked()),this,SLOT(getSender()));
  // debug("<!---- %s ---->\n",ui->pushButton_general->objectName().toStdString().c_str());
  QObject::connect(ui->pushButton_general_time,SIGNAL(clicked()),this,SLOT(getSender()));
  QObject::connect(ui->pushButton_general_user,SIGNAL(clicked()),this,SLOT(getSender()));

  Connector *con2=new Connector(ui->stackedWidget_channel,this);
  QObject::connect(ui->pushButton_channel_preview,SIGNAL(clicked()),con2,SLOT(currentIndex0()));
  QObject::connect(ui->pushButton_channel_stream,SIGNAL(clicked()),con2,SLOT(currentIndex1()));
  QObject::connect(ui->pushButton_channel_videoSet,SIGNAL(clicked()),con2,SLOT(currentIndex2()));
  QObject::connect(ui->pushButton_channel_audioSet,SIGNAL(clicked()),con2,SLOT(currentIndex3()));

  QObject::connect(ui->pushButton_channel_preview,SIGNAL(clicked()),this,SLOT(getSender()));
  QObject::connect(ui->pushButton_channel_stream,SIGNAL(clicked()),this,SLOT(getSender()));
  QObject::connect(ui->pushButton_channel_videoSet,SIGNAL(clicked()),this,SLOT(getSender()));
  QObject::connect(ui->pushButton_channel_audioSet,SIGNAL(clicked()),this,SLOT(getSender()));

  Connector *con3=new Connector(ui->stackedWidget_network,this);
  QObject::connect(ui->pushButton_network_wifi,SIGNAL(clicked()),con3,SLOT(currentIndex0()));
  QObject::connect(ui->pushButton_network_wireless,SIGNAL(clicked()),con3,SLOT(currentIndex1()));
  QObject::connect(ui->pushButton_network_service,SIGNAL(clicked()),con3,SLOT(currentIndex2()));

  QObject::connect(ui->pushButton_network_wifi,SIGNAL(clicked()),this,SLOT(getSender()));
  QObject::connect(ui->pushButton_network_wireless,SIGNAL(clicked()),this,SLOT(getSender()));
  QObject::connect(ui->pushButton_network_service,SIGNAL(clicked()),this,SLOT(getSender()));

  Connector *con4=new Connector(ui->stackedWidget_warn,this);
  QObject::connect(ui->pushButton_warn_email,SIGNAL(clicked()),con4,SLOT(currentIndex0()));
  QObject::connect(ui->pushButton_warn_moveTest,SIGNAL(clicked()),con4,SLOT(currentIndex1()));
  QObject::connect(ui->pushButton_warn_videoLost,SIGNAL(clicked()),con4,SLOT(currentIndex2()));
  QObject::connect(ui->pushButton_warn_deviceWarn,SIGNAL(clicked()),con4,SLOT(currentIndex3()));

  QObject::connect(ui->pushButton_warn_email,SIGNAL(clicked()),this,SLOT(getSender()));
  QObject::connect(ui->pushButton_warn_moveTest,SIGNAL(clicked()),this,SLOT(getSender()));
  QObject::connect(ui->pushButton_warn_videoLost,SIGNAL(clicked()),this,SLOT(getSender()));
  QObject::connect(ui->pushButton_warn_deviceWarn,SIGNAL(clicked()),this,SLOT(getSender()));

  Connector *con5=new Connector(ui->stackedWidget_picManager,this);
  QObject::connect(ui->pushButton_picManager_picManager,SIGNAL(clicked()),con5,SLOT(currentIndex0()));

  QObject::connect(ui->pushButton_picManager_picManager,SIGNAL(clicked()),this,SLOT(getSender()));

  Connector *con6=new Connector(ui->stackedWidget_system,this);
  QObject::connect(ui->pushButton_system_log,SIGNAL(clicked()),con6,SLOT(currentIndex0()));
  QObject::connect(ui->pushButton_system_weihu,SIGNAL(clicked()),con6,SLOT(currentIndex1()));
  QObject::connect(ui->pushButton_system_info,SIGNAL(clicked()),con6,SLOT(currentIndex2()));

  QObject::connect(ui->pushButton_system_log,SIGNAL(clicked()),this,SLOT(getSender()));
  QObject::connect(ui->pushButton_system_weihu,SIGNAL(clicked()),this,SLOT(getSender()));
  QObject::connect(ui->pushButton_system_info,SIGNAL(clicked()),this,SLOT(getSender()));

  Connector *con7=new Connector(ui->stackedWidget_other,this);
  QObject::connect(ui->pushButton_other_memory,SIGNAL(clicked()),con7,SLOT(currentIndex0()));
  QObject::connect(ui->pushButton_other_yuntai,SIGNAL(clicked()),con7,SLOT(currentIndex1()));

  QObject::connect(ui->pushButton_other_memory,SIGNAL(clicked()),this,SLOT(getSender()));
  QObject::connect(ui->pushButton_other_yuntai,SIGNAL(clicked()),this,SLOT(getSender()));

  QObject::connect(ui->listWidget_email_list, SIGNAL(currentRowChanged(int)),this, SLOT(curRow(int)));

  QObject::connect(ui->pushButton_other_yuntai,SIGNAL(clicked()),this,SLOT(pushButtonOtherYuntai()));
  QObject::connect(ui->pushButton_other_memory,SIGNAL(clicked()),this,SLOT(pushButtonOtherMemory()));
  QObject::connect(ui->comboBox_memory_diskSize,SIGNAL(activated(int)),this,SLOT(comboBoxMemoryDiskSize(int)));
  QObject::connect(ui->comboBox_yuntai_channelId,SIGNAL(activated(int)),this,SLOT(comboBoxYuntaiChannelId(int)));
  QObject::connect(ui->comboBox_yuntai_baudRate,SIGNAL(activated(int)),this,SLOT(comboBoxYuntaiBaudRate(int)));
  QObject::connect(ui->comboBox_yuntai_protocol,SIGNAL(activated(int)),this,SLOT(comboBoxYuntaiProtocol(int)));
  QObject::connect(ui->comboBox_yuntai_addr,SIGNAL(activated(int)),this,SLOT(comboBoxYuntaiAddr(int)));
  QObject::connect(ui->checkBox_yuntai_leftRightExchange,SIGNAL(stateChanged(int)),this,SLOT(checkBoxYuntaiLeftRightExchange(int)));
  QObject::connect(ui->checkBox_yuntai_upDownExchange,SIGNAL(stateChanged(int)),this,SLOT(checkBoxYuntaiUpDownExchange(int)));
  QObject::connect(ui->checkBox_yuntai_stopExchange,SIGNAL(stateChanged(int)),this,SLOT(checkBoxYuntaiStopExchange(int)));
  QObject::connect(ui->checkBox_yuntai_zoomedExchange,SIGNAL(stateChanged(int)),this,SLOT(checkBoxYuntaiZoomedExchange(int)));
  QObject::connect(ui->checkBox_yuntai_zoomExchange,SIGNAL(stateChanged(int)),this,SLOT(checkBoxYuntaiZoomExchange(int)));
  QObject::connect(ui->pushButton_other_use,SIGNAL(clicked()),this,SLOT(pushButtonOtherUse()));
  QObject::connect(ui->pushButton_other_sure,SIGNAL(clicked()),this,SLOT(pushButtonOtherSure()));
  QObject::connect(ui->pushButton_other_cancel,SIGNAL(clicked()),this,SLOT(pushButtonOtherCancel()));
  QObject::connect(ui->pushButton_system_info,SIGNAL(clicked()),this,SLOT(pushButtonSystemInfo()));
  QObject::connect(ui->pushButton_system_log,SIGNAL(clicked()),this,SLOT(pushButtonSystemLog()));
  QObject::connect(ui->pushButton_system_weihu,SIGNAL(clicked()),this,SLOT(pushButtonSystemWeihu()));
  QObject::connect(ui->comboBox_log_type,SIGNAL(activated(int)),this,SLOT(comboBoxLogType(int)));
  QObject::connect(ui->checkBox_log_date,SIGNAL(stateChanged(int)),this,SLOT(checkBoxLogDate(int)));
  QObject::connect(ui->pushButton_log_backupToU,SIGNAL(clicked()),this,SLOT(pushButtonLogBackupToU()));
  QObject::connect(ui->checkBox_weihu_restart,SIGNAL(stateChanged(int)),this,SLOT(checkBoxWeihuRestart(int)));
  QObject::connect(ui->comboBox_weihu_updateType,SIGNAL(activated(int)),this,SLOT(comboBoxWeihuUpdateType(int)));
  QObject::connect(ui->comboBox_weihu_server,SIGNAL(activated(int)),this,SLOT(comboBoxWeihuServer(int)));
  QObject::connect(ui->pushButton_weihu_importConfig,SIGNAL(clicked()),this,SLOT(pushButtonWeihuImportConfig()));
  QObject::connect(ui->pushButton_weihu_exportConfig,SIGNAL(clicked()),this,SLOT(pushButtonWeihuExportConfig()));
  QObject::connect(ui->pushButton_weihu_update,SIGNAL(clicked()),this,SLOT(pushButtonWeihuUpdate()));
  QObject::connect(ui->pushButton_weihu_register,SIGNAL(clicked()),this,SLOT(pushButtonWeihuRegister()));
  QObject::connect(ui->pushButton_weihu_formatAll,SIGNAL(clicked()),this,SLOT(pushButtonWeihuFormatAll()));
  QObject::connect(ui->pushButton_system_sure,SIGNAL(clicked()),this,SLOT(pushButtonSystemSure()));
  QObject::connect(ui->pushButton_system_use,SIGNAL(clicked()),this,SLOT(pushButtonSystemUse()));
  QObject::connect(ui->pushButton_system_cancel,SIGNAL(clicked()),this,SLOT(pushButtonSystemCancel()));
  QObject::connect(ui->pushButton_picManager_picManager,SIGNAL(clicked()),this,SLOT(pushButtonPicManagerPicManager()));
  QObject::connect(ui->comboBox_picManager_findType,SIGNAL(activated(int)),this,SLOT(comboBoxPicManagerFindType(int)));
  QObject::connect(ui->comboBox_picManager_channelId,SIGNAL(activated(int)),this,SLOT(comboBoxPicManagerChannelId(int)));
  QObject::connect(ui->pushButton_picManager_selectAll,SIGNAL(clicked()),this,SLOT(pushButtonPicManagerSelectAll()));
  QObject::connect(ui->pushButton_picManager_del,SIGNAL(clicked()),this,SLOT(pushButtonPicManagerDel()));
  QObject::connect(ui->pushButton_picManager_backupToU,SIGNAL(clicked()),this,SLOT(pushButtonPicManagerBackupToU()));
  QObject::connect(ui->pushButton_picManager_find,SIGNAL(clicked()),this,SLOT(pushButtonPicManagerFind()));
  QObject::connect(ui->pushButton_picManager_sure,SIGNAL(clicked()),this,SLOT(pushButtonPicManagerSure()));
  QObject::connect(ui->pushButton_picManager_use,SIGNAL(clicked()),this,SLOT(pushButtonPicManagerUse()));
  QObject::connect(ui->pushButton_picManager_cancel,SIGNAL(clicked()),this,SLOT(pushButtonPicManagerCancel()));
  QObject::connect(ui->pushButton_warn_deviceWarn,SIGNAL(clicked()),this,SLOT(pushButtonWarnDeviceWarn()));
  QObject::connect(ui->pushButton_warn_videoLost,SIGNAL(clicked()),this,SLOT(pushButtonWarnVideoLost()));
  QObject::connect(ui->pushButton_warn_moveTest,SIGNAL(clicked()),this,SLOT(pushButtonWarnMoveTest()));
  QObject::connect(ui->pushButton_warn_email,SIGNAL(clicked()),this,SLOT(pushButtonWarnEmail()));
  QObject::connect(ui->pushButton_videoLost_selectAll,SIGNAL(clicked()),this,SLOT(pushButtonVideoLostSelectAll()));
  QObject::connect(ui->pushButton_videoLost_default,SIGNAL(clicked()),this,SLOT(pushButtonVideoLostDefault()));
  QObject::connect(ui->pushButton_videoLost_warnOut_1,SIGNAL(clicked()),this,SLOT(pushButtonVideoLostWarnOut1()));
  QObject::connect(ui->checkBox_videoLost_open_1,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoLostOpen1(int)));
  QObject::connect(ui->checkBox_videoLost_warnOrder_1,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoLostWarnOrder1(int)));
  QObject::connect(ui->pushButton_videoLost_warnOut_2,SIGNAL(clicked()),this,SLOT(pushButtonVideoLostWarnOut2()));
  QObject::connect(ui->checkBox_videoLost_open_2,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoLostOpen2(int)));
  QObject::connect(ui->checkBox_videoLost_warnOrder_2,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoLostWarnOrder2(int)));
  QObject::connect(ui->pushButton_videoLost_warnOut_3,SIGNAL(clicked()),this,SLOT(pushButtonVideoLostWarnOut3()));
  QObject::connect(ui->pushButton_videoLost_warnOut_4,SIGNAL(clicked()),this,SLOT(pushButtonVideoLostWarnOut4()));
  QObject::connect(ui->checkBox_videoLost_warnOrder_4,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoLostWarnOrder4(int)));
  QObject::connect(ui->checkBox_videoLost_open_4,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoLostOpen4(int)));
  QObject::connect(ui->checkBox_videoLost_open_3,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoLostOpen3(int)));
  QObject::connect(ui->checkBox_videoLost_warnOrder_3,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoLostWarnOrder3(int)));
  QObject::connect(ui->pushButton_videoLost_warnOut_5,SIGNAL(clicked()),this,SLOT(pushButtonVideoLostWarnOut5()));
  QObject::connect(ui->pushButton_videoLost_warnOut_6,SIGNAL(clicked()),this,SLOT(pushButtonVideoLostWarnOut6()));
  QObject::connect(ui->checkBox_videoLost_warnOrder_6,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoLostWarnOrder6(int)));
  QObject::connect(ui->checkBox_videoLost_open_6,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoLostOpen6(int)));
  QObject::connect(ui->checkBox_videoLost_open_5,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoLostOpen5(int)));
  QObject::connect(ui->checkBox_videoLost_warnOrder_5,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoLostWarnOrder5(int)));
  QObject::connect(ui->pushButton_videoLost_warnOut_7,SIGNAL(clicked()),this,SLOT(pushButtonVideoLostWarnOut7()));
  QObject::connect(ui->pushButton_videoLost_warnOut_8,SIGNAL(clicked()),this,SLOT(pushButtonVideoLostWarnOut8()));
  QObject::connect(ui->checkBox_videoLost_warnOrder_8,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoLostWarnOrder8(int)));
  QObject::connect(ui->checkBox_videoLost_open_8,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoLostOpen8(int)));
  QObject::connect(ui->checkBox_videoLost_open_7,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoLostOpen7(int)));
  QObject::connect(ui->checkBox_videoLost_warnOrder_7,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoLostWarnOrder7(int)));
  QObject::connect(ui->pushButton_moveTest_selectAll,SIGNAL(clicked()),this,SLOT(pushButtonMoveTestSelectAll()));
  QObject::connect(ui->pushButton_moveTest_default,SIGNAL(clicked()),this,SLOT(pushButtonMoveTestDefault()));
  QObject::connect(ui->checkBox_moveTest_open_1,SIGNAL(stateChanged(int)),this,SLOT(checkBoxMoveTestOpen1(int)));
  QObject::connect(ui->checkBox_moveTest_warnOrder_1,SIGNAL(stateChanged(int)),this,SLOT(checkBoxMoveTestWarnOrder1(int)));
  QObject::connect(ui->pushButton_moveTest_areaSet_1,SIGNAL(clicked()),this,SLOT(pushButtonMoveTestAreaSet1()));
  QObject::connect(ui->pushButton_moveTest_warnOut_1,SIGNAL(clicked()),this,SLOT(pushButtonMoveTestWarnOut1()));
  QObject::connect(ui->checkBox_moveTest_warnOrder_2,SIGNAL(stateChanged(int)),this,SLOT(checkBoxMoveTestWarnOrder2(int)));
  QObject::connect(ui->checkBox_moveTest_open_2,SIGNAL(stateChanged(int)),this,SLOT(checkBoxMoveTestOpen2(int)));
  QObject::connect(ui->pushButton_moveTest_areaSet_2,SIGNAL(clicked()),this,SLOT(pushButtonMoveTestAreaSet2()));
  QObject::connect(ui->pushButton_moveTest_warnOut_2,SIGNAL(clicked()),this,SLOT(pushButtonMoveTestWarnOut2()));
  QObject::connect(ui->checkBox_moveTest_warnOrder_3,SIGNAL(stateChanged(int)),this,SLOT(checkBoxMoveTestWarnOrder3(int)));
  QObject::connect(ui->checkBox_moveTest_open_3,SIGNAL(stateChanged(int)),this,SLOT(checkBoxMoveTestOpen3(int)));
  QObject::connect(ui->pushButton_moveTest_warnOut_3,SIGNAL(clicked()),this,SLOT(pushButtonMoveTestWarnOut3()));
  QObject::connect(ui->pushButton_moveTest_areaSet_3,SIGNAL(clicked()),this,SLOT(pushButtonMoveTestAreaSet3()));
  QObject::connect(ui->checkBox_moveTest_warnOrder_4,SIGNAL(stateChanged(int)),this,SLOT(checkBoxMoveTestWarnOrder4(int)));
  QObject::connect(ui->checkBox_moveTest_open_4,SIGNAL(stateChanged(int)),this,SLOT(checkBoxMoveTestOpen4(int)));
  QObject::connect(ui->pushButton_moveTest_warnOut_4,SIGNAL(clicked()),this,SLOT(pushButtonMoveTestWarnOut4()));
  QObject::connect(ui->pushButton_moveTest_areaSet_4,SIGNAL(clicked()),this,SLOT(pushButtonMoveTestAreaSet4()));
  QObject::connect(ui->checkBox_moveTest_warnOrder_5,SIGNAL(stateChanged(int)),this,SLOT(checkBoxMoveTestWarnOrder5(int)));
  QObject::connect(ui->checkBox_moveTest_open_5,SIGNAL(stateChanged(int)),this,SLOT(checkBoxMoveTestOpen5(int)));
  QObject::connect(ui->pushButton_moveTest_warnOut_5,SIGNAL(clicked()),this,SLOT(pushButtonMoveTestWarnOut5()));
  QObject::connect(ui->pushButton_moveTest_areaSet_5,SIGNAL(clicked()),this,SLOT(pushButtonMoveTestAreaSet5()));
  QObject::connect(ui->checkBox_moveTest_warnOrder_6,SIGNAL(stateChanged(int)),this,SLOT(checkBoxMoveTestWarnOrder6(int)));
  QObject::connect(ui->checkBox_moveTest_open_6,SIGNAL(stateChanged(int)),this,SLOT(checkBoxMoveTestOpen6(int)));
  QObject::connect(ui->pushButton_moveTest_warnOut_6,SIGNAL(clicked()),this,SLOT(pushButtonMoveTestWarnOut6()));
  QObject::connect(ui->pushButton_moveTest_areaSet_6,SIGNAL(clicked()),this,SLOT(pushButtonMoveTestAreaSet6()));
  QObject::connect(ui->checkBox_moveTest_warnOrder_7,SIGNAL(stateChanged(int)),this,SLOT(checkBoxMoveTestWarnOrder7(int)));
  QObject::connect(ui->checkBox_moveTest_open_7,SIGNAL(stateChanged(int)),this,SLOT(checkBoxMoveTestOpen7(int)));
  QObject::connect(ui->pushButton_moveTest_warnOut_7,SIGNAL(clicked()),this,SLOT(pushButtonMoveTestWarnOut7()));
  QObject::connect(ui->pushButton_moveTest_areaSet_7,SIGNAL(clicked()),this,SLOT(pushButtonMoveTestAreaSet7()));
  QObject::connect(ui->checkBox_moveTest_warnOrder_8,SIGNAL(stateChanged(int)),this,SLOT(checkBoxMoveTestWarnOrder8(int)));
  QObject::connect(ui->checkBox_moveTest_open_8,SIGNAL(stateChanged(int)),this,SLOT(checkBoxMoveTestOpen8(int)));
  QObject::connect(ui->pushButton_moveTest_warnOut_8,SIGNAL(clicked()),this,SLOT(pushButtonMoveTestWarnOut8()));
  QObject::connect(ui->pushButton_moveTest_areaSet_8,SIGNAL(clicked()),this,SLOT(pushButtonMoveTestAreaSet8()));
  QObject::connect(ui->pushButton_email_addList,SIGNAL(clicked()),this,SLOT(pushButtonEmailAddList()));
  QObject::connect(ui->pushButton_email_editList,SIGNAL(clicked()),this,SLOT(pushButtonEmailEditList()));
  QObject::connect(ui->pushButton_email_delList,SIGNAL(clicked()),this,SLOT(pushButtonEmailDelList()));
  QObject::connect(ui->pushButton_email_serverPort,SIGNAL(clicked()),this,SLOT(pushButtonEmailServerPort()));
  QObject::connect(ui->comboBox_email_server,SIGNAL(activated(int)),this,SLOT(comboBoxEmailServer(int)));
  QObject::connect(ui->comboBox_email_safeType,SIGNAL(activated(int)),this,SLOT(comboBoxEmailSafeType(int)));
  QObject::connect(ui->pushButton_email_test,SIGNAL(clicked()),this,SLOT(pushButtonEmailTest()));
  QObject::connect(ui->pushButton_deviceWarn_selectAll,SIGNAL(clicked()),this,SLOT(pushButtonDeviceWarnSelectAll()));
  QObject::connect(ui->pushButton_deviceWarn_default,SIGNAL(clicked()),this,SLOT(pushButtonDeviceWarnDefault()));
  QObject::connect(ui->checkBox_deviceWarn_open_1,SIGNAL(stateChanged(int)),this,SLOT(checkBoxDeviceWarnOpen1(int)));
  QObject::connect(ui->checkBox_deviceWarn_warnOrder_1,SIGNAL(stateChanged(int)),this,SLOT(checkBoxDeviceWarnWarnOrder1(int)));
  QObject::connect(ui->pushButton_deviceWarn_warnOut_1,SIGNAL(clicked()),this,SLOT(pushButtonDeviceWarnWarnOut1()));
  QObject::connect(ui->comboBox_deviceWarn_funcModel_1,SIGNAL(activated(int)),this,SLOT(comboBoxDeviceWarnFuncModel1(int)));
  QObject::connect(ui->checkBox_deviceWarn_open_2,SIGNAL(stateChanged(int)),this,SLOT(checkBoxDeviceWarnOpen2(int)));
  QObject::connect(ui->checkBox_deviceWarn_warnOrder_2,SIGNAL(stateChanged(int)),this,SLOT(checkBoxDeviceWarnWarnOrder2(int)));
  QObject::connect(ui->comboBox_deviceWarn_funcModel_2,SIGNAL(activated(int)),this,SLOT(comboBoxDeviceWarnFuncModel2(int)));
  QObject::connect(ui->pushButton_deviceWarn_warnOut_2,SIGNAL(clicked()),this,SLOT(pushButtonDeviceWarnWarnOut2()));
  QObject::connect(ui->checkBox_deviceWarn_warnOrder_4,SIGNAL(stateChanged(int)),this,SLOT(checkBoxDeviceWarnWarnOrder4(int)));
  QObject::connect(ui->checkBox_deviceWarn_open_4,SIGNAL(stateChanged(int)),this,SLOT(checkBoxDeviceWarnOpen4(int)));
  QObject::connect(ui->checkBox_deviceWarn_open_3,SIGNAL(stateChanged(int)),this,SLOT(checkBoxDeviceWarnOpen3(int)));
  QObject::connect(ui->checkBox_deviceWarn_warnOrder_3,SIGNAL(stateChanged(int)),this,SLOT(checkBoxDeviceWarnWarnOrder3(int)));
  QObject::connect(ui->comboBox_deviceWarn_funcModel_4,SIGNAL(activated(int)),this,SLOT(comboBoxDeviceWarnFuncModel4(int)));
  QObject::connect(ui->comboBox_deviceWarn_funcModel_3,SIGNAL(activated(int)),this,SLOT(comboBoxDeviceWarnFuncModel3(int)));
  QObject::connect(ui->pushButton_deviceWarn_warnOut_3,SIGNAL(clicked()),this,SLOT(pushButtonDeviceWarnWarnOut3()));
  QObject::connect(ui->pushButton_deviceWarn_warnOut_4,SIGNAL(clicked()),this,SLOT(pushButtonDeviceWarnWarnOut4()));
  QObject::connect(ui->checkBox_deviceWarn_warnOrder_6,SIGNAL(stateChanged(int)),this,SLOT(checkBoxDeviceWarnWarnOrder6(int)));
  QObject::connect(ui->checkBox_deviceWarn_open_6,SIGNAL(stateChanged(int)),this,SLOT(checkBoxDeviceWarnOpen6(int)));
  QObject::connect(ui->checkBox_deviceWarn_open_5,SIGNAL(stateChanged(int)),this,SLOT(checkBoxDeviceWarnOpen5(int)));
  QObject::connect(ui->checkBox_deviceWarn_warnOrder_5,SIGNAL(stateChanged(int)),this,SLOT(checkBoxDeviceWarnWarnOrder5(int)));
  QObject::connect(ui->comboBox_deviceWarn_funcModel_6,SIGNAL(activated(int)),this,SLOT(comboBoxDeviceWarnFuncModel6(int)));
  QObject::connect(ui->comboBox_deviceWarn_funcModel_5,SIGNAL(activated(int)),this,SLOT(comboBoxDeviceWarnFuncModel5(int)));
  QObject::connect(ui->pushButton_deviceWarn_warnOut_5,SIGNAL(clicked()),this,SLOT(pushButtonDeviceWarnWarnOut5()));
  QObject::connect(ui->pushButton_deviceWarn_warnOut_6,SIGNAL(clicked()),this,SLOT(pushButtonDeviceWarnWarnOut6()));
  QObject::connect(ui->checkBox_deviceWarn_warnOrder_8,SIGNAL(stateChanged(int)),this,SLOT(checkBoxDeviceWarnWarnOrder8(int)));
  QObject::connect(ui->checkBox_deviceWarn_open_8,SIGNAL(stateChanged(int)),this,SLOT(checkBoxDeviceWarnOpen8(int)));
  QObject::connect(ui->checkBox_deviceWarn_open_7,SIGNAL(stateChanged(int)),this,SLOT(checkBoxDeviceWarnOpen7(int)));
  QObject::connect(ui->checkBox_deviceWarn_warnOrder_7,SIGNAL(stateChanged(int)),this,SLOT(checkBoxDeviceWarnWarnOrder7(int)));
  QObject::connect(ui->comboBox_deviceWarn_funcModel_8,SIGNAL(activated(int)),this,SLOT(comboBoxDeviceWarnFuncModel8(int)));
  QObject::connect(ui->comboBox_deviceWarn_funcModel_7,SIGNAL(activated(int)),this,SLOT(comboBoxDeviceWarnFuncModel7(int)));
  QObject::connect(ui->pushButton_deviceWarn_warnOut_7,SIGNAL(clicked()),this,SLOT(pushButtonDeviceWarnWarnOut7()));
  QObject::connect(ui->pushButton_deviceWarn_warnOut_8,SIGNAL(clicked()),this,SLOT(pushButtonDeviceWarnWarnOut8()));
  QObject::connect(ui->pushButton_warn_sure,SIGNAL(clicked()),this,SLOT(pushButtonWarnSure()));
  QObject::connect(ui->pushButton_warn_use,SIGNAL(clicked()),this,SLOT(pushButtonWarnUse()));
  QObject::connect(ui->pushButton_warn_cancel,SIGNAL(clicked()),this,SLOT(pushButtonWarnCancel()));
  QObject::connect(ui->pushButton_network_service,SIGNAL(clicked()),this,SLOT(pushButtonNetworkService()));
  QObject::connect(ui->pushButton_network_wifi,SIGNAL(clicked()),this,SLOT(pushButtonNetworkWifi()));
  QObject::connect(ui->pushButton_network_wireless,SIGNAL(clicked()),this,SLOT(pushButtonNetworkWireless()));
  QObject::connect(ui->checkBox_wireless_dhcp,SIGNAL(stateChanged(int)),this,SLOT(checkBoxWirelessDhcp(int)));
  QObject::connect(ui->pushButton_wifi_ADSLConnect,SIGNAL(clicked()),this,SLOT(pushButtonWifiADSLConnect()));
  QObject::connect(ui->pushButton_wifi_ADSLDisconnect,SIGNAL(clicked()),this,SLOT(pushButtonWifiADSLDisconnect()));
  QObject::connect(ui->pushButton_wifi_cardDisconnect,SIGNAL(clicked()),this,SLOT(pushButtonWifiCardDisconnect()));
  QObject::connect(ui->pushButton_wifi_cardConnect,SIGNAL(clicked()),this,SLOT(pushButtonWifiCardConnect()));
  QObject::connect(ui->pushButton_network_cancel,SIGNAL(clicked()),this,SLOT(pushButtonNetworkCancel()));
  QObject::connect(ui->pushButton_network_sure,SIGNAL(clicked()),this,SLOT(pushButtonNetworkSure()));
  QObject::connect(ui->pushButton_network_use,SIGNAL(clicked()),this,SLOT(pushButtonNetworkUse()));
  QObject::connect(ui->pushButton_channel_stream,SIGNAL(clicked()),this,SLOT(pushButtonChannelStream()));
  QObject::connect(ui->pushButton_channel_audioSet,SIGNAL(clicked()),this,SLOT(pushButtonChannelAudioSet()));
  QObject::connect(ui->pushButton_channel_videoSet,SIGNAL(clicked()),this,SLOT(pushButtonChannelVideoSet()));
  QObject::connect(ui->pushButton_channel_preview,SIGNAL(clicked()),this,SLOT(pushButtonChannelPreview()));
  QObject::connect(ui->pushButton_videoSet_videoCopy,SIGNAL(clicked()),this,SLOT(pushButtonVideoSetVideoCopy()));
  QObject::connect(ui->pushButton_videoSet_default,SIGNAL(clicked()),this,SLOT(pushButtonVideoSetDefault()));
  QObject::connect(ui->checkBox_videoSet_myselfVideo_1,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoSetMyselfVideo1(int)));
  QObject::connect(ui->checkBox_videoSet_timingVideoTwo_1,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoSetTimingVideoTwo1(int)));
  QObject::connect(ui->checkBox_videoSet_timingVideoOne_1,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoSetTimingVideoOne1(int)));
  QObject::connect(ui->checkBox_videoSet_myselfVideo_2,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoSetMyselfVideo2(int)));
  QObject::connect(ui->checkBox_videoSet_timingVideoOne_2,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoSetTimingVideoOne2(int)));
  QObject::connect(ui->checkBox_videoSet_timingVideoTwo_2,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoSetTimingVideoTwo2(int)));
  QObject::connect(ui->checkBox_videoSet_myselfVideo_3,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoSetMyselfVideo3(int)));
  QObject::connect(ui->checkBox_videoSet_myselfVideo_4,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoSetMyselfVideo4(int)));
  QObject::connect(ui->checkBox_videoSet_timingVideoOne_3,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoSetTimingVideoOne3(int)));
  QObject::connect(ui->checkBox_videoSet_timingVideoTwo_3,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoSetTimingVideoTwo3(int)));
  QObject::connect(ui->checkBox_videoSet_timingVideoTwo_4,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoSetTimingVideoTwo4(int)));
  QObject::connect(ui->checkBox_videoSet_timingVideoOne_4,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoSetTimingVideoOne4(int)));
  QObject::connect(ui->checkBox_videoSet_myselfVideo_5,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoSetMyselfVideo5(int)));
  QObject::connect(ui->checkBox_videoSet_myselfVideo_6,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoSetMyselfVideo6(int)));
  QObject::connect(ui->checkBox_videoSet_timingVideoOne_5,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoSetTimingVideoOne5(int)));
  QObject::connect(ui->checkBox_videoSet_timingVideoTwo_5,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoSetTimingVideoTwo5(int)));
  QObject::connect(ui->checkBox_videoSet_timingVideoTwo_6,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoSetTimingVideoTwo6(int)));
  QObject::connect(ui->checkBox_videoSet_timingVideoOne_6,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoSetTimingVideoOne6(int)));
  QObject::connect(ui->checkBox_videoSet_myselfVideo_7,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoSetMyselfVideo7(int)));
  QObject::connect(ui->checkBox_videoSet_myselfVideo_8,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoSetMyselfVideo8(int)));
  QObject::connect(ui->checkBox_videoSet_timingVideoOne_7,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoSetTimingVideoOne7(int)));
  QObject::connect(ui->checkBox_videoSet_timingVideoTwo_7,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoSetTimingVideoTwo7(int)));
  QObject::connect(ui->checkBox_videoSet_timingVideoTwo_8,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoSetTimingVideoTwo8(int)));
  QObject::connect(ui->checkBox_videoSet_timingVideoOne_8,SIGNAL(stateChanged(int)),this,SLOT(checkBoxVideoSetTimingVideoOne8(int)));
  QObject::connect(ui->comboBox_stream_minorFenBianLv_1,SIGNAL(activated(int)),this,SLOT(comboBoxStreamMinorFenBianLv1(int)));
  QObject::connect(ui->comboBox_stream_fenbialv_1,SIGNAL(activated(int)),this,SLOT(comboBoxStreamFenbialv1(int)));
  QObject::connect(ui->comboBox_stream_liuChangDu_1,SIGNAL(activated(int)),this,SLOT(comboBoxStreamLiuChangDu1(int)));
  QObject::connect(ui->comboBox_stream_liuChangDu_2,SIGNAL(activated(int)),this,SLOT(comboBoxStreamLiuChangDu2(int)));
  QObject::connect(ui->comboBox_stream_minorFenBianLv_2,SIGNAL(activated(int)),this,SLOT(comboBoxStreamMinorFenBianLv2(int)));
  QObject::connect(ui->comboBox_stream_fenbialv_2,SIGNAL(activated(int)),this,SLOT(comboBoxStreamFenbialv2(int)));
  QObject::connect(ui->comboBox_stream_fenbialv_4,SIGNAL(activated(int)),this,SLOT(comboBoxStreamFenbialv4(int)));
  QObject::connect(ui->comboBox_stream_liuChangDu_4,SIGNAL(activated(int)),this,SLOT(comboBoxStreamLiuChangDu4(int)));
  QObject::connect(ui->comboBox_stream_liuChangDu_3,SIGNAL(activated(int)),this,SLOT(comboBoxStreamLiuChangDu3(int)));
  QObject::connect(ui->comboBox_stream_minorFenBianLv_4,SIGNAL(activated(int)),this,SLOT(comboBoxStreamMinorFenBianLv4(int)));
  QObject::connect(ui->comboBox_stream_minorFenBianLv_3,SIGNAL(activated(int)),this,SLOT(comboBoxStreamMinorFenBianLv3(int)));
  QObject::connect(ui->comboBox_stream_fenbialv_3,SIGNAL(activated(int)),this,SLOT(comboBoxStreamFenbialv3(int)));
  QObject::connect(ui->comboBox_stream_fenbialv_6,SIGNAL(activated(int)),this,SLOT(comboBoxStreamFenbialv6(int)));
  QObject::connect(ui->comboBox_stream_liuChangDu_6,SIGNAL(activated(int)),this,SLOT(comboBoxStreamLiuChangDu6(int)));
  QObject::connect(ui->comboBox_stream_liuChangDu_5,SIGNAL(activated(int)),this,SLOT(comboBoxStreamLiuChangDu5(int)));
  QObject::connect(ui->comboBox_stream_minorFenBianLv_6,SIGNAL(activated(int)),this,SLOT(comboBoxStreamMinorFenBianLv6(int)));
  QObject::connect(ui->comboBox_stream_minorFenBianLv_5,SIGNAL(activated(int)),this,SLOT(comboBoxStreamMinorFenBianLv5(int)));
  QObject::connect(ui->comboBox_stream_fenbialv_5,SIGNAL(activated(int)),this,SLOT(comboBoxStreamFenbialv5(int)));
  QObject::connect(ui->comboBox_stream_fenbialv_8,SIGNAL(activated(int)),this,SLOT(comboBoxStreamFenbialv8(int)));
  QObject::connect(ui->comboBox_stream_liuChangDu_8,SIGNAL(activated(int)),this,SLOT(comboBoxStreamLiuChangDu8(int)));
  QObject::connect(ui->comboBox_stream_liuChangDu_7,SIGNAL(activated(int)),this,SLOT(comboBoxStreamLiuChangDu7(int)));
  QObject::connect(ui->comboBox_stream_minorFenBianLv_8,SIGNAL(activated(int)),this,SLOT(comboBoxStreamMinorFenBianLv8(int)));
  QObject::connect(ui->comboBox_stream_minorFenBianLv_7,SIGNAL(activated(int)),this,SLOT(comboBoxStreamMinorFenBianLv7(int)));
  QObject::connect(ui->comboBox_stream_fenbialv_7,SIGNAL(activated(int)),this,SLOT(comboBoxStreamFenbialv7(int)));
  QObject::connect(ui->pushButton_preview_copy,SIGNAL(clicked()),this,SLOT(pushButtonPreviewCopy()));
  QObject::connect(ui->pushButton_preview_setDefault,SIGNAL(clicked()),this,SLOT(pushButtonPreviewSetDefault()));
  QObject::connect(ui->pushButton_preview_channelSort,SIGNAL(clicked()),this,SLOT(pushButtonPreviewChannelSort()));
  QObject::connect(ui->checkBox_preview_arenHide_1,SIGNAL(stateChanged(int)),this,SLOT(checkBoxPreviewArenHide1(int)));
  QObject::connect(ui->pushButton_preview_areaHideSet_1,SIGNAL(clicked()),this,SLOT(pushButtonPreviewAreaHideSet1()));
  QObject::connect(ui->checkBox_preview_hide_1,SIGNAL(stateChanged(int)),this,SLOT(checkBoxPreviewHide1(int)));
  QObject::connect(ui->comboBox_preview_OSD_1,SIGNAL(activated(int)),this,SLOT(comboBoxPreviewOSD1(int)));
  QObject::connect(ui->checkBox_preview_arenHide_2,SIGNAL(stateChanged(int)),this,SLOT(checkBoxPreviewArenHide2(int)));
  QObject::connect(ui->pushButton_preview_areaHideSet_2,SIGNAL(clicked()),this,SLOT(pushButtonPreviewAreaHideSet2()));
  QObject::connect(ui->checkBox_preview_hide_2,SIGNAL(stateChanged(int)),this,SLOT(checkBoxPreviewHide2(int)));
  QObject::connect(ui->comboBox_preview_OSD_2,SIGNAL(activated(int)),this,SLOT(comboBoxPreviewOSD2(int)));
  QObject::connect(ui->checkBox_preview_arenHide_3,SIGNAL(stateChanged(int)),this,SLOT(checkBoxPreviewArenHide3(int)));
  QObject::connect(ui->pushButton_preview_areaHideSet_3,SIGNAL(clicked()),this,SLOT(pushButtonPreviewAreaHideSet3()));
  QObject::connect(ui->checkBox_preview_hide_3,SIGNAL(stateChanged(int)),this,SLOT(checkBoxPreviewHide3(int)));
  QObject::connect(ui->comboBox_preview_OSD_3,SIGNAL(activated(int)),this,SLOT(comboBoxPreviewOSD3(int)));
  QObject::connect(ui->checkBox_preview_arenHide_4,SIGNAL(stateChanged(int)),this,SLOT(checkBoxPreviewArenHide4(int)));
  QObject::connect(ui->pushButton_preview_areaHideSet_4,SIGNAL(clicked()),this,SLOT(pushButtonPreviewAreaHideSet4()));
  QObject::connect(ui->checkBox_preview_hide_4,SIGNAL(stateChanged(int)),this,SLOT(checkBoxPreviewHide4(int)));
  QObject::connect(ui->comboBox_preview_OSD_4,SIGNAL(activated(int)),this,SLOT(comboBoxPreviewOSD4(int)));
  QObject::connect(ui->checkBox_preview_arenHide_5,SIGNAL(stateChanged(int)),this,SLOT(checkBoxPreviewArenHide5(int)));
  QObject::connect(ui->pushButton_preview_areaHideSet_5,SIGNAL(clicked()),this,SLOT(pushButtonPreviewAreaHideSet5()));
  QObject::connect(ui->checkBox_preview_hide_5,SIGNAL(stateChanged(int)),this,SLOT(checkBoxPreviewHide5(int)));
  QObject::connect(ui->comboBox_preview_OSD_5,SIGNAL(activated(int)),this,SLOT(comboBoxPreviewOSD5(int)));
  QObject::connect(ui->checkBox_preview_arenHide_6,SIGNAL(stateChanged(int)),this,SLOT(checkBoxPreviewArenHide6(int)));
  QObject::connect(ui->pushButton_preview_areaHideSet_6,SIGNAL(clicked()),this,SLOT(pushButtonPreviewAreaHideSet6()));
  QObject::connect(ui->checkBox_preview_hide_6,SIGNAL(stateChanged(int)),this,SLOT(checkBoxPreviewHide6(int)));
  QObject::connect(ui->comboBox_preview_OSD_6,SIGNAL(activated(int)),this,SLOT(comboBoxPreviewOSD6(int)));
  QObject::connect(ui->checkBox_preview_arenHide_7,SIGNAL(stateChanged(int)),this,SLOT(checkBoxPreviewArenHide7(int)));
  QObject::connect(ui->pushButton_preview_areaHideSet_7,SIGNAL(clicked()),this,SLOT(pushButtonPreviewAreaHideSet7()));
  QObject::connect(ui->checkBox_preview_hide_7,SIGNAL(stateChanged(int)),this,SLOT(checkBoxPreviewHide7(int)));
  QObject::connect(ui->comboBox_preview_OSD_7,SIGNAL(activated(int)),this,SLOT(comboBoxPreviewOSD7(int)));
  QObject::connect(ui->checkBox_preview_arenHide_8,SIGNAL(stateChanged(int)),this,SLOT(checkBoxPreviewArenHide8(int)));
  QObject::connect(ui->pushButton_preview_areaHideSet_8,SIGNAL(clicked()),this,SLOT(pushButtonPreviewAreaHideSet8()));
  QObject::connect(ui->checkBox_preview_hide_8,SIGNAL(stateChanged(int)),this,SLOT(checkBoxPreviewHide8(int)));
  QObject::connect(ui->comboBox_preview_OSD_8,SIGNAL(activated(int)),this,SLOT(comboBoxPreviewOSD8(int)));
  QObject::connect(ui->comboBox_audioSet_audioZoom_1,SIGNAL(activated(int)),this,SLOT(comboBoxAudioSetAudioZoom1(int)));
  QObject::connect(ui->checkBox_audioSet_audio_1,SIGNAL(stateChanged(int)),this,SLOT(checkBoxAudioSetAudio1(int)));
  QObject::connect(ui->comboBox_audioSet_bindVideo_1,SIGNAL(activated(int)),this,SLOT(comboBoxAudioSetBindVideo1(int)));
  QObject::connect(ui->checkBox_audioSet_audio_2,SIGNAL(stateChanged(int)),this,SLOT(checkBoxAudioSetAudio2(int)));
  QObject::connect(ui->comboBox_audioSet_bindVideo_2,SIGNAL(activated(int)),this,SLOT(comboBoxAudioSetBindVideo2(int)));
  QObject::connect(ui->comboBox_audioSet_audioZoom_2,SIGNAL(activated(int)),this,SLOT(comboBoxAudioSetAudioZoom2(int)));
  QObject::connect(ui->checkBox_audioSet_audio_3,SIGNAL(stateChanged(int)),this,SLOT(checkBoxAudioSetAudio3(int)));
  QObject::connect(ui->comboBox_audioSet_bindVideo_4,SIGNAL(activated(int)),this,SLOT(comboBoxAudioSetBindVideo4(int)));
  QObject::connect(ui->comboBox_audioSet_audioZoom_4,SIGNAL(activated(int)),this,SLOT(comboBoxAudioSetAudioZoom4(int)));
  QObject::connect(ui->checkBox_audioSet_audio_4,SIGNAL(stateChanged(int)),this,SLOT(checkBoxAudioSetAudio4(int)));
  QObject::connect(ui->comboBox_audioSet_bindVideo_3,SIGNAL(activated(int)),this,SLOT(comboBoxAudioSetBindVideo3(int)));
  QObject::connect(ui->comboBox_audioSet_audioZoom_3,SIGNAL(activated(int)),this,SLOT(comboBoxAudioSetAudioZoom3(int)));
  QObject::connect(ui->pushButton_channel_sure,SIGNAL(clicked()),this,SLOT(pushButtonChannelSure()));
  QObject::connect(ui->pushButton_channnel_use,SIGNAL(clicked()),this,SLOT(pushButtonChannnelUse()));
  QObject::connect(ui->pushButton_channel_cancel,SIGNAL(clicked()),this,SLOT(pushButtonChannelCancel()));
  QObject::connect(ui->pushButton_general_subGeneral,SIGNAL(clicked()),this,SLOT(pushButtonGeneralSubGeneral()));
  QObject::connect(ui->pushButton_general_time,SIGNAL(clicked()),this,SLOT(pushButtonGeneralTime()));
  QObject::connect(ui->pushButton_general_user,SIGNAL(clicked()),this,SLOT(pushButtonGeneralUser()));
  QObject::connect(ui->comboBox_user_userList,SIGNAL(activated(int)),this,SLOT(comboBoxUserUserList(int)));
  QObject::connect(ui->pushButton_user_userAdd,SIGNAL(clicked()),this,SLOT(pushButtonUserUserAdd()));
  QObject::connect(ui->pushButton_user_userEdit,SIGNAL(clicked()),this,SLOT(pushButtonUserUserEdit()));
  QObject::connect(ui->pushButton_user_userDel,SIGNAL(clicked()),this,SLOT(pushButtonUserUserDel()));
  QObject::connect(ui->checkBox_subGeneral_guide,SIGNAL(stateChanged(int)),this,SLOT(checkBoxSubGeneralGuide(int)));
  QObject::connect(ui->comboBox_subGeneral_transparent,SIGNAL(activated(int)),this,SLOT(comboBoxSubGeneralTransparent(int)));
  QObject::connect(ui->comboBox_subGeneral_VGAResolutionRatio,SIGNAL(activated(int)),this,SLOT(comboBoxSubGeneralVGAResolutionRatio(int)));
  QObject::connect(ui->comboBox_subGeneral_sensorData,SIGNAL(activated(int)),this,SLOT(comboBoxSubGeneralSensorData(int)));
  QObject::connect(ui->checkBox_time_autoGetTime,SIGNAL(stateChanged(int)),this,SLOT(checkBoxTimeAutoGetTime(int)));
  QObject::connect(ui->comboBox_time_timeFormat,SIGNAL(activated(int)),this,SLOT(comboBoxTimeTimeFormat(int)));
  QObject::connect(ui->comboBox_time_timeArea,SIGNAL(activated(int)),this,SLOT(comboBoxTimeTimeArea(int)));
  QObject::connect(ui->pushButton_general_sure,SIGNAL(clicked()),this,SLOT(pushButtonGeneralSure()));
  QObject::connect(ui->pushButton_general_cancel,SIGNAL(clicked()),this,SLOT(pushButtonGeneralCancel()));
  QObject::connect(ui->pushButton_general_use,SIGNAL(clicked()),this,SLOT(pushButtonGeneralUse()));

  ui->lineEdit_log_selectAll->installEventFilter(this);
  ui->lineEdit_log_date->installEventFilter(this);
  ui->lineEdit_weihu_restart->installEventFilter(this);
  ui->lineEdit_picManager_open->installEventFilter(this);
  ui->lineEdit_picManager_end->installEventFilter(this);
  ui->lineEdit_email_serverPort->installEventFilter(this);
  ui->lineEdit_email_userName->installEventFilter(this);
  ui->lineEdit_email_userpassword->installEventFilter(this);
  ui->lineEdit_email_interval->installEventFilter(this);
  ui->lineEdit_wireless_ip->installEventFilter(this);
  ui->lineEdit_wireless_gateway->installEventFilter(this);
  ui->lineEdit_wireless_dns->installEventFilter(this);
  ui->lineEdit_wireless_mask->installEventFilter(this);
  ui->lineEdit_wireless_mac->installEventFilter(this);
  ui->lineEdit_wifi_ADSLName->installEventFilter(this);
  ui->lineEdit_wifi_ADSLPassword->installEventFilter(this);
  ui->lineEdit_wifi_cardPassword->installEventFilter(this);
  ui->lineEdit_wifi_cardName->installEventFilter(this);
  ui->lineEdit_service_videoForwardingAddr->installEventFilter(this);
  ui->lineEdit_service_videoForwardingPort->installEventFilter(this);
  ui->lineEdit_service_videoUploadPort->installEventFilter(this);
  ui->lineEdit_service_videoUploadAddr->installEventFilter(this);
  ui->lineEdit_stream_minorRate_1->installEventFilter(this);
  ui->lineEdit_stream_mainRate_1->installEventFilter(this);
  ui->lineEdit_stream_mainRate_2->installEventFilter(this);
  ui->lineEdit_stream_minorRate_2->installEventFilter(this);
  ui->lineEdit_stream_mainRate_3->installEventFilter(this);
  ui->lineEdit_stream_mainRate_4->installEventFilter(this);
  ui->lineEdit_stream_minorRate_3->installEventFilter(this);
  ui->lineEdit_stream_minorRate_4->installEventFilter(this);
  ui->lineEdit_stream_mainRate_5->installEventFilter(this);
  ui->lineEdit_stream_mainRate_6->installEventFilter(this);
  ui->lineEdit_stream_minorRate_5->installEventFilter(this);
  ui->lineEdit_stream_minorRate_6->installEventFilter(this);
  ui->lineEdit_stream_mainRate_7->installEventFilter(this);
  ui->lineEdit_stream_mainRate_8->installEventFilter(this);
  ui->lineEdit_stream_minorRate_7->installEventFilter(this);
  ui->lineEdit_stream_minorRate_8->installEventFilter(this);
  ui->lineEdit_preview_channelName_1->installEventFilter(this);
  ui->lineEdit_preview_channelName_2->installEventFilter(this);
  ui->lineEdit_preview_channelName_3->installEventFilter(this);
  ui->lineEdit_preview_channelName_4->installEventFilter(this);
  ui->lineEdit_preview_channelName_5->installEventFilter(this);
  ui->lineEdit_preview_channelName_6->installEventFilter(this);
  ui->lineEdit_preview_channelName_7->installEventFilter(this);
  ui->lineEdit_preview_channelName_8->installEventFilter(this);
  ui->lineEdit_audioSet_listen_1->installEventFilter(this);
  ui->lineEdit_audioSet_listen_2->installEventFilter(this);
  ui->lineEdit_audioSet_listen_3->installEventFilter(this);
  ui->lineEdit_audioSet_listen_4->installEventFilter(this);
  ui->lineEdit_user_autoLoginOut->installEventFilter(this);
  ui->lineEdit_time_dateSet->installEventFilter(this);
  ui->lineEdit_time_timeSet->installEventFilter(this);

  loadQssFile("qss/SecondDialog.qss");
}

void dialog_null_systemSetDialog::loadQssFile(QString fileName)
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
void dialog_null_systemSetDialog::generalSubGeneral()
{
  static bool first=true;
  QString message;
  QString myXmalHead="<SystemSet>";
  QString myXmlEnd="</SystemSet>";
  message.clear();
  message.append(myXmalHead);
  if(first){
      message.append("<General><SubGeneral method=\"Get\"></SubGeneral></General>");
      first=false;
      message.append(myXmlEnd);
      emit sendMessageToProxy(message);
      return;
    }

  QString myXmlSubHead="<General><SubGeneral method=\"Set\">";
  QString myXmlSubEnd="</SubGeneral></General>";
  message.append(myXmlSubHead);

  message.append("<SensorData>"+ui->comboBox_subGeneral_sensorData->currentText()+"</SensorData>");
  message.append("<Transparency>"+ui->comboBox_subGeneral_transparent->currentText()+"</Transparency>");
  message.append("<VGAResolution>"+ui->comboBox_subGeneral_VGAResolutionRatio->currentText()+"</VGAResolution>");
  message.append("<BootTip>"+QString::number(ui->checkBox_subGeneral_guide->checkState())+"</BootTip>");

  message.append(myXmlSubEnd);
  message.append(myXmlEnd);
  emit sendMessageToProxy(message);

  oldName=ui->pushButton_general_subGeneral->objectName();
}
void dialog_null_systemSetDialog::generalTime()
{
  static bool first=true;
  QString message;
  QString myXmalHead="<SystemSet>";
  QString myXmlEnd="</SystemSet>";
  message.clear();
  message.append(myXmalHead);
  if(first){
      message.append("<General><Datetime method=\"Get\"></Datetime></General>");
      first=false;
      message.append(myXmlEnd);
      emit sendMessageToProxy(message);
      return;
    }

  QString myXmlSubHead="<General><DateTime method=\"Set\">";
  QString myXmlSubEnd="</DateTime></General>";
  message.append(myXmlSubHead);

  message.append("<Dateformat>"+ui->comboBox_time_timeFormat->currentText()+"</Dateformat>");
  message.append("<DateSet>"+ui->lineEdit_time_dateSet->text()+"</DateSet>");
  message.append("<TimeSet>"+ui->lineEdit_time_timeSet->text()+"</TimeSet>");
  message.append("<TimeArea>"+ui->comboBox_time_timeArea->currentText()+"</TimeArea>");
  message.append("<AutoGetTime>"+QString::number(ui->checkBox_time_autoGetTime->checkState())+"</AutoGetTime>");

  message.append(myXmlSubEnd);
  message.append(myXmlEnd);
  emit sendMessageToProxy(message);

  oldName=ui->pushButton_general_time->objectName();
}
void dialog_null_systemSetDialog::generalUser()
{
  static bool first=true;
  QString message;
  QString myXmalHead="<SystemSet>";
  QString myXmlEnd="</SystemSet>";
  message.clear();
  message.append(myXmalHead);
  if(first){
      message.append("<General><User method=\"Get\"></User></General>");
      first=false;
      message.append(myXmlEnd);
      emit sendMessageToProxy(message);
      return;
    }

  QString myXmlSubHead="<General><User method=\"Set\">";
  QString myXmlSubEnd="</User></General>";
  message.append(myXmlSubHead);

  message.append(myXmlSubEnd);
  message.append(myXmlEnd);
  //emit sendMessageToProxy(message);
  oldName=ui->pushButton_general_use->objectName();
}
void dialog_null_systemSetDialog::channelPreview()
{
  static bool first=true;
  QString message;
  QString myXmalHead="<SystemSet>";
  QString myXmlEnd="</SystemSet>";
  message.clear();
  message.append(myXmalHead);
  if(first){
      message.append("<Channel><Preview method=\"Get\"></Preview></Channel>");
      first=false;
      message.append(myXmlEnd);
      emit sendMessageToProxy(message);
      return;
    }

  message.append(myXmlEnd);
  emit sendMessageToProxy(message);
  oldName=ui->pushButton_channel_preview->objectName();
}
void dialog_null_systemSetDialog::channelStream()
{
  static bool first=true;
  QString message;
  QString myXmalHead="<SystemSet>";
  QString myXmlEnd="</SystemSet>";
  message.clear();
  message.append(myXmalHead);
  if(first){
      message.append("<Channel><Stream method=\"Get\"></Stream></Channel>");
      first=false;
      message.append(myXmlEnd);
      emit sendMessageToProxy(message);
      return;
    }

  message.append(myXmlEnd);
  emit sendMessageToProxy(message);
  oldName=ui->pushButton_channel_stream->objectName();

}
void dialog_null_systemSetDialog::channelVideoSet()
{
  static bool first=true;
  QString message;
  QString myXmalHead="<SystemSet>";
  QString myXmlEnd="</SystemSet>";
  message.clear();
  message.append(myXmalHead);
  if(first){
      message.append("<Channel><Video method=\"Get\"></Video></Channel>");
      first=false;
      message.append(myXmlEnd);
      emit sendMessageToProxy(message);
      return;
    }

  message.append(myXmlEnd);
  emit sendMessageToProxy(message);
  oldName=ui->pushButton_channel_videoSet->objectName();

}
void dialog_null_systemSetDialog::channelAudioSet()
{
  static bool first=true;
  QString message;
  QString myXmalHead="<SystemSet>";
  QString myXmlEnd="</SystemSet>";
  message.clear();
  message.append(myXmalHead);
  if(first){
      message.append("<Channel><Audio method=\"Get\"></Audio></Channel>");
      first=false;
      message.append(myXmlEnd);
      emit sendMessageToProxy(message);
      return;
    }

  message.append(myXmlEnd);
  emit sendMessageToProxy(message);
  oldName=ui->pushButton_channel_audioSet->objectName();
}
void dialog_null_systemSetDialog::networkWifi()
{
  static bool first=true;
  QString message;
  QString myXmalHead="<SystemSet>";
  QString myXmlEnd="</SystemSet>";
  message.clear();
  message.append(myXmalHead);
  if(first){
      message.append("<Network><Wifi method=\"Get\"></Wifi></Network>");
      first=false;
      message.append(myXmlEnd);
      emit sendMessageToProxy(message);
      return;
    }

  message.append(myXmlEnd);
  emit sendMessageToProxy(message);
  oldName=ui->pushButton_network_wifi->objectName();
}
void dialog_null_systemSetDialog::networkService()
{
  static bool first=true;
  QString message;
  QString myXmalHead="<SystemSet>";
  QString myXmlEnd="</SystemSet>";
  message.clear();
  message.append(myXmalHead);
  if(first){
      message.append("<Network><Service method=\"Get\"></Service></Network>");
      first=false;
      message.append(myXmlEnd);
      emit sendMessageToProxy(message);
      return;
    }

  message.append(myXmlEnd);
  emit sendMessageToProxy(message);
  oldName=ui->pushButton_network_service->objectName();

}
void dialog_null_systemSetDialog::networkWireless()
{
  static bool first=true;
  QString message;
  QString myXmalHead="<SystemSet>";
  QString myXmlEnd="</SystemSet>";
  message.clear();
  message.append(myXmalHead);
  if(first){
      message.append("<Network><Wired  method=\"Get\"></Wired></Network>");
      first=false;
      message.append(myXmlEnd);
      emit sendMessageToProxy(message);
      return;
    }

  message.append(myXmlEnd);
  emit sendMessageToProxy(message);
  oldName=ui->pushButton_network_wireless->objectName();

}
void dialog_null_systemSetDialog::warnEmail()
{
  static bool first=true;
  QString message;
  QString myXmalHead="<SystemSet>";
  QString myXmlEnd="</SystemSet>";
  message.clear();
  message.append(myXmalHead);
  if(first){
      message.append("<WarnSet><EmailSet method=\"Get\"></EmailSet></WarnSet>");
      first=false;
      message.append(myXmlEnd);
      emit sendMessageToProxy(message);
      return;
    }

  message.append(myXmlEnd);
  emit sendMessageToProxy(message);
  oldName=ui->pushButton_warn_email->objectName();

}
void dialog_null_systemSetDialog::warnMoveTest()
{
  static bool first=true;
  QString message;
  QString myXmalHead="<SystemSet>";
  QString myXmlEnd="</SystemSet>";
  message.clear();
  message.append(myXmalHead);
  if(first){
      message.append("<WarnSet><MobileMonitor method=\"Get\"></MobileMonitor></WarnSet>");
      first=false;
      message.append(myXmlEnd);
      emit sendMessageToProxy(message);
      return;
    }

  message.append(myXmlEnd);
  emit sendMessageToProxy(message);
  oldName=ui->pushButton_warn_moveTest->objectName();
}
void dialog_null_systemSetDialog::warnVideoLost()
{
  static bool first=true;
  QString message;
  QString myXmalHead="<SystemSet>";
  QString myXmlEnd="</SystemSet>";
  message.clear();
  message.append(myXmalHead);
  if(first){
      message.append("<WarnSet><VideoLost method=\"Get\"></VideoLost></WarnSet>");
      first=false;
      message.append(myXmlEnd);
      emit sendMessageToProxy(message);
      return;
    }


  message.append(myXmlEnd);
  emit sendMessageToProxy(message);
  oldName=ui->pushButton_warn_videoLost->objectName();
}
void dialog_null_systemSetDialog::warnDeviceWarn()
{
  static bool first=true;
  QString message;
  QString myXmalHead="<SystemSet>";
  QString myXmlEnd="</SystemSet>";
  message.clear();
  message.append(myXmalHead);
  if(first){
      message.append("<WarnSet><DeviceWarn method=\"Get\"></DeviceWarn></WarnSet>");
      first=false;
      message.append(myXmlEnd);
      emit sendMessageToProxy(message);
      return;
    }


  message.append(myXmlEnd);
  emit sendMessageToProxy(message);
  oldName=ui->pushButton_warn_deviceWarn->objectName();

}
void dialog_null_systemSetDialog::picManagerPicManager()
{
  static bool first=true;
  QString message;
  QString myXmalHead="<SystemSet>";
  QString myXmlEnd="</SystemSet>";
  message.clear();
  message.append(myXmalHead);
  if(first){
      message.append("<Picture><PicManager method=\"Get\"></PicManager></Picture>");
      first=false;
      message.append(myXmlEnd);
      emit sendMessageToProxy(message);
      return;
    }

  message.append(myXmlEnd);
  emit sendMessageToProxy(message);
  oldName=ui->pushButton_picManager_picManager->objectName();
}
void dialog_null_systemSetDialog::systemLog()
{
  static bool first=true;
  QString message;
  QString myXmalHead="<SystemSet>";
  QString myXmlEnd="</SystemSet>";
  message.clear();
  message.append(myXmalHead);
  if(first){
      message.append("<System><Log method=\"Get\"></Log></System>");
      first=false;
      message.append(myXmlEnd);
      emit sendMessageToProxy(message);
      return;
    }

  message.append(myXmlEnd);
  emit sendMessageToProxy(message);
  oldName=ui->pushButton_system_log->objectName();
}
void dialog_null_systemSetDialog::systemWeihu()
{
  static bool first=true;
  QString message;
  QString myXmalHead="<SystemSet>";
  QString myXmlEnd="</SystemSet>";
  message.clear();
  message.append(myXmalHead);
  if(first){
      message.append("<System><Maintain method=\"Get\"></Maintain></System>");
      first=false;
      message.append(myXmlEnd);
      emit sendMessageToProxy(message);
      return;
    }

  message.append(myXmlEnd);
  emit sendMessageToProxy(message);
  oldName=ui->pushButton_system_weihu->objectName();
}
void dialog_null_systemSetDialog::systemInfo()
{
  static bool first=true;
  QString message;
  QString myXmalHead="<SystemSet>";
  QString myXmlEnd="</SystemSet>";
  message.clear();
  message.append(myXmalHead);
  if(first){
      message.append("<System><info method=\"Get\"></info></System>");
      first=false;
      message.append(myXmlEnd);
      emit sendMessageToProxy(message);
      return;
    }

  message.append(myXmlEnd);
  emit sendMessageToProxy(message);
  oldName=ui->pushButton_system_info->objectName();
}
void dialog_null_systemSetDialog::otherMemory()
{
  static bool first=true;
  QString message;
  QString myXmalHead="<SystemSet>";
  QString myXmlEnd="</SystemSet>";
  message.clear();
  message.append(myXmalHead);
  if(first){
      message.append("<Other><MemoryDevice method=\"Get\"></MemoryDevice></Other>");
      first=false;
      message.append(myXmlEnd);

      emit sendMessageToProxy(message);
      return;
    }

  message.append(myXmlEnd);
  emit sendMessageToProxy(message);
  oldName=ui->pushButton_other_memory->objectName();
}
void dialog_null_systemSetDialog::otherYuntai()
{
  static bool first=true;
  QString message;
  QString myXmalHead="<SystemSet>";
  QString myXmlEnd="</SystemSet>";
  message.clear();
  message.append(myXmalHead);
  if(first){
      message.append("<Other><YunTaiSet method=\"Get\"></YunTaiSet></Other>");
      first=false;
      message.append(myXmlEnd);
      emit sendMessageToProxy(message);
      return;
    }

  message.append(myXmlEnd);
  emit sendMessageToProxy(message);
  oldName=ui->pushButton_other_yuntai->objectName();
}
/*此处返回发如果有Code则是 Set动作,若此处返回码没有Code 则是 Get操作*/
void dialog_null_systemSetDialog::startXml(QString message)
{
  //  debug("%s\n",message.toStdString().c_str());
  //添加XML解析
  QDomDocument doc;
  doc.setContent(message);
  QDomElement docElem = doc.documentElement();  //返回根元素
  QDomElement n = docElem.firstChildElement();   //返回根节点的第一个子节点

  QDomElement e=n.firstChildElement();
  if(e.tagName()=="General"){
      QDomElement qe=e.firstChildElement();
      if(qe.tagName()=="SubGeneral"){
          QDomElement child=qe.firstChildElement();
          while(!child.isNull()){
              if(child.isElement()){
                  if(child.tagName()=="SensorData"){
                      setComboValue(qe.attribute("method"),child.text(),ui->comboBox_subGeneral_sensorData);
                    }else if(child.tagName()=="Transparency"){
                      setComboValue(qe.attribute("method"),child.text(),ui->comboBox_subGeneral_transparent);
                    }else if(child.tagName()=="VGAResolution"){
                      setComboValue(qe.attribute("method"),child.text(),ui->comboBox_subGeneral_VGAResolutionRatio);
                    }else if(child.tagName()=="BootTip"){
                      if(child.text().toInt()!=0){
                          ui->checkBox_subGeneral_guide->setChecked(true);
                        }else{
                          ui->checkBox_subGeneral_guide->setChecked(false);
                        }
                    }
                }
              child=child.nextSiblingElement();
            }
        }else if(qe.tagName()=="Datetime"){
          QDomElement child=qe.firstChildElement();
          while(!child.isNull()){
              if(child.isElement()){
                  if(child.tagName()=="Dateformat"){
                      setComboValue(qe.attribute("method"),child.text(), ui->comboBox_time_timeFormat);
                    }else if(child.tagName()=="DateSet"){
                      ui->lineEdit_time_dateSet->setText(child.text());
                    }else if(child.tagName()=="TimeSet"){
                      ui->lineEdit_time_timeSet->setText(child.text());
                    }else if(child.tagName()=="TimeArea"){
                      setComboValue(qe.attribute("method"),child.text(), ui->comboBox_time_timeArea);
                    }else if(child.tagName()=="AutoGetTime"){
                      if(child.text().toInt()!=0){
                          ui->checkBox_time_autoGetTime->setChecked(true);
                        }else{
                          ui->checkBox_time_autoGetTime->setChecked(false);
                        }
                    }
                }
              child=child.nextSiblingElement();
            }
        } else if(qe.tagName()=="User"){
          QDomElement child=qe.firstChildElement();
          while(!child.isNull()){
              if(child.isElement()){
                  if(child.tagName()=="UserNameList"){
                      setComboValue(qe.attribute("method"),child.text(), ui->comboBox_user_userList);
                    }else if(child.tagName()=="AuotLoginoutTime"){
                      ui->lineEdit_user_autoLoginOut->setText(child.text());
                    }
                }
              child=child.nextSiblingElement();
            }
        }else if(qe.tagName()=="Code"){
          int code=qe.text().toInt();
        }
    }else if(e.tagName()=="Channel"){
      QDomElement qe=e.firstChildElement();
      if(e.tagName()=="Preview"){
          QDomElement child=qe.firstChildElement();
          int index=0;
          while(!child.isNull()){
              if(child.tagName()=="ChannelId"){
                  if(child.text().toInt()>8 || child.text().toInt()<1){
                      break;
                    }else{
                      index=child.text().toInt();
                    }
                }else if(child.tagName()=="ChannelName"){
                  QString objectName="lineEdit_preview_channelName_"+QString::number(index);
                  QLineEdit *edit=(QLineEdit*)widgetMap.find(objectName).value();;
                  edit->setText(child.text());
                }else if(child.tagName()=="Osd"){
                  QString objectName="comboBox_preview_OSD_"+QString::number(index);
                  QComboBox *combo=(QComboBox *)widgetMap.find(objectName).value();;
                  setComboValue(qe.attribute("method"),child.text(),combo);
                }else if(child.tagName()=="Hide"){
                  QString objectName="checkBox_preview_hide_"+QString::number(index);
                  QCheckBox *check=(QCheckBox *)widgetMap.find(objectName).value();;
                  if(child.text().toInt()!=0){
                      check->setChecked(true);
                    }else{
                      check->setChecked(false);
                    }
                }else if(child.tagName()=="HideArea"){
                  QString objectName="checkBox_preview_areahide_"+QString::number(index);
                  QCheckBox *check=(QCheckBox *)widgetMap.find(objectName).value();;
                  if(child.text().toInt()!=0){
                      check->setChecked(true);
                      QString pushObjectName="pushButton_preview_areaHideSet_"+QString::number(index);
                      QPushButton *button=(QPushButton *)widgetMap.find(pushObjectName).value();
                      button->setVisible(true);
                    }else{
                      check->setChecked(false);
                      QString pushObjectName="pushButton_preview_areaHideSet_"+QString::number(index);
                      QPushButton *button=(QPushButton *)widgetMap.find(pushObjectName).value();
                      button->setVisible(false);
                    }
                }
              child=child.nextSiblingElement();
            }
        }else if(qe.tagName()=="Stream"){
          QDomElement child=qe.firstChildElement();
          int index=0;
          while(!child.isNull()){
              if(child.tagName()=="ChannelId"){
                  if(child.text().toInt()>8 || child.text().toInt()<1){
                      break;
                    }else{
                      index=child.text().toInt();
                    }
                }else if(qe.tagName()=="MajorResolution"){
                  QString name="comboBox_stream_fenbialv_"+QString::number(index);
                  QComboBox *combo=(QComboBox *)widgetMap.find(name).value();;
                  setComboValue(qe.attribute("method"),child.text(),combo);
                }else if(qe.tagName()=="MajorRate"){
                  QString name="lineEdit_stream_mainRate_"+QString::number(index);
                  QLineEdit *edit=(QLineEdit *)widgetMap.find(name).value();
                  edit->setText(child.text());
                }else if(qe.tagName()=="MinorResolution"){
                  QString name="comboBox_stream_minorFenBianLv_"+QString::number(index);
                  QComboBox *combo=(QComboBox *)widgetMap.find(name).value();;
                  setComboValue(qe.attribute("method"),child.text(),combo);
                }else if(qe.tagName()=="MinorRate"){
                  QString name="lineEdit_stream_minorRate_"+QString::number(index);
                  QLineEdit *edit=(QLineEdit *)widgetMap.find(name).value();;
                  edit->setText(child.text());
                }else if(qe.tagName()=="Fluent"){
                  QString name="comboBox_stream_liuChangDu_"+QString::number(index);
                  QComboBox *combo=(QComboBox *)widgetMap.find(name).value();
                  setComboValue(qe.attribute("method"),child.text(),combo);
                }
              child=child.nextSiblingElement();
            }
        }else if(qe.tagName()=="Video"){
          QDomElement child=qe.firstChildElement();
          int index=0;
          while(!child.isNull()){
              if(child.tagName()=="ChannelId"){
                  if(child.text().toInt()>8 || child.text().toInt()<1){
                      break;
                    }else{
                      index=child.text().toInt();
                    }
                }else if(qe.tagName()=="Auto"){
                  QString name="checkBox_videoSet_myselfVideo_"+QString::number(index);
                  QCheckBox *box=(QCheckBox*)widgetMap.find(name).value();
                  if(qe.text().toInt()!=0){
                      box->setChecked(true);
                    }else{
                      box->setChecked(false);
                    }
                }else if(qe.tagName()=="VideoTime1"){
                  QString name="checkBox_videoSet_myselfVideo_"+QString::number(index);
                  QCheckBox *box=(QCheckBox*)widgetMap.find(name).value();
                  if(qe.text().toInt()!=0){
                      box->setChecked(true);
                    }else{
                      box->setChecked(false);
                    }
                }else if(qe.tagName()=="VideoTime2"){
                  QString name="checkBox_videoSet_myselfVideo_"+QString::number(index);
                  QCheckBox *box=(QCheckBox*)widgetMap.find(name).value();
                  if(qe.text().toInt()!=0){
                      box->setChecked(true);
                    }else{
                      box->setChecked(false);
                    }
                }

              child=child.nextSiblingElement();
            }
        }else if(qe.tagName()=="Audio"){

        }else if(qe.tagName()=="Code"){

        }
    }else if(e.tagName()=="Network"){
      QDomElement qe=e.firstChildElement();
      if(qe.tagName()=="Wifi"){
        }else if(qe.tagName()=="Wired"){
          QDomElement child=qe.firstChildElement();
          while(!child.isNull()){
              if(child.isElement()){
                  if(child.tagName()=="Dhcp"){
                      if(child.text().toInt()!=0){
                          ui->checkBox_wireless_dhcp->setChecked(true);
                          ui->lineEdit_wireless_dns->setVisible(false);
                          ui->lineEdit_wireless_gateway->setVisible(false);
                          ui->lineEdit_wireless_ip->setVisible(false);
                          ui->lineEdit_wireless_mac->setVisible(false);
                          ui->lineEdit_wireless_mask->setVisible(false);
                        }else{
                          ui->checkBox_wireless_dhcp->setChecked(false);
                          ui->lineEdit_wireless_dns->setVisible(true);
                          ui->lineEdit_wireless_gateway->setVisible(true);
                          ui->lineEdit_wireless_ip->setVisible(true);
                          ui->lineEdit_wireless_mac->setVisible(true);
                          ui->lineEdit_wireless_mask->setVisible(true);
                        }
                    }else if(child.tagName()=="Ip"){
                      ui->lineEdit_wireless_ip->setText(child.text());
                    }else if(child.tagName()=="Mask"){
                      ui->lineEdit_wireless_mask->setText(child.text());
                    }else if(child.tagName()=="Gateway"){
                      ui->lineEdit_wireless_gateway->setText(child.text());
                    }else if(child.tagName()=="Dns"){
                      ui->lineEdit_wireless_dns->setText(child.text());
                    }else if(child.tagName()=="Mac"){
                      ui->lineEdit_wireless_mac->setText(child.text());
                    }
                }
              child=child.nextSiblingElement();
            }
        }else if(qe.tagName()=="Service"){
          QDomElement child=qe.firstChildElement();
          while(!child.isNull()){
              if(child.isElement()){
                  if(child.tagName()=="VideoRelayAddr"){
                      ui->lineEdit_service_videoForwardingAddr->setText(child.text());
                    }else if(child.tagName()=="VidoRelayPort"){
                      ui->lineEdit_service_videoForwardingPort->setText(child.text());
                    }else if(child.tagName()=="VidoUploadAddr"){
                      ui->lineEdit_service_videoUploadAddr->setText(child.text());
                    }else if(child.tagName()=="VideoUploadPort"){
                      ui->lineEdit_service_videoUploadPort->setText(child.text());
                    }
                }
              child=child.nextSiblingElement();
            }
        }else if(qe.tagName()=="Code"){
        }
    }else if(e.tagName()=="WarnSet"){
      QDomElement qe=e.firstChildElement();
      if(qe.tagName()=="EmailSet"){
          QDomElement child=qe.firstChildElement();
          while(!child.isNull()){
              if(child.isElement()){
                  if(child.tagName()=="EditReciver"){
                      QStringList nameList=child.text().split(":");
                      for(int i=0;i<nameList.size();++i){
                          dialog_null_emailSetSubDialog  *reciver=new dialog_null_emailSetSubDialog(ui->listWidget_email_list);
                          reciver->getLabe()->setText(nameList.at(i));
                          QListWidgetItem *item=new QListWidgetItem(ui->listWidget_email_list);
                          ui->listWidget_email_list->setItemWidget(item,reciver);
                          item->setSizeHint (QSize(reciver->rect().width(),reciver->rect().height()));
                        }
                    }else if(child.tagName()=="EditSender"){
                      QDomElement subChild=child.firstChildElement();
                      while(!subChild.isNull()){
                          if(subChild.isElement()){
                              if(subChild.tagName()=="EmailServer"){
                                  setComboValue(qe.attribute("method"),child.text(), ui->comboBox_email_server);
                                }else if(subChild.tagName()=="Port"){
                                  ui->lineEdit_email_serverPort->setText(subChild.text());
                                }else if(subChild.tagName()=="Type"){
                                  setComboValue(qe.attribute("method"),child.text(),ui->comboBox_email_safeType);
                                }else if(subChild.tagName()=="UserName"){
                                  ui->lineEdit_email_userName->setText(subChild.text());
                                }else if(subChild.tagName()=="UserPassword"){
                                  ui->lineEdit_email_userpassword->setText(subChild.text());
                                }else if(subChild.tagName()=="SendTimeout"){
                                  ui->lineEdit_email_interval->setText(subChild.text());
                                }else if(subChild.tagName()=="EmailTest"){
                                }
                            }
                          subChild=subChild.nextSiblingElement();
                        }
                    }
                }
              child=child.nextSiblingElement();
            }
        }else if(qe.tagName()=="MobileMonitor"){
          QDomElement child=qe.firstChildElement();
          int index;
          while(!child.isNull()){
              if(child.isElement()){
                  if(child.tagName()=="ChannelId"){
                      if(child.text().toInt()>8 || child.text().toInt()<1){
                          break;
                        }else{
                          index=child.text().toInt();
                        }
                    }else if(child.tagName()=="Open"){
                      QString name="checkBox_moveTest_open_"+QString::number(index);
                      QCheckBox *box=(QCheckBox *)widgetMap.find(name).value();
                      if(child.text().toInt()!=0){
                          box->setChecked(true);
                        }else{
                          box->setChecked(false);
                        }
                    }else if(child.tagName()=="WarnOrder"){
                      QString name="checkBox_moveTest_warnOrder_"+QString::number(index);
                      QCheckBox *box=(QCheckBox *)widgetMap.find(name).value();
                      if(child.text().toInt()!=0){
                          box->setChecked(true);
                        }else{
                          box->setChecked(false);
                        }
                    }
                }
              child=child.nextSiblingElement();
            }

        }else if(qe.tagName()=="VideoLost"){
          QDomElement child=qe.firstChildElement();
          int index;
          while(!child.isNull()){
              if(child.isElement()){
                  if(child.tagName()=="ChannelId"){
                      if(child.text().toInt()>8 || child.text().toInt()<1){
                          break;
                        }else{
                          index=child.text().toInt();
                        }
                    }else if(child.tagName()=="Open"){
                      QString name="checkBox_videoLost_open_"+QString::number(index);
                      QCheckBox *box=(QCheckBox *)widgetMap.find(name).value();
                      if(child.text().toInt()!=0){
                          box->setChecked(true);
                        }else{
                          box->setChecked(false);
                        }
                    }else if(child.tagName()=="WarnOrder"){
                      QString name="checkBox_videoLost_warnOrder_"+QString::number(index);
                      QCheckBox *box=(QCheckBox *)widgetMap.find(name).value();
                      if(child.text().toInt()!=0){
                          box->setChecked(true);
                        }else{
                          box->setChecked(false);
                        }
                    }
                }
              child=child.nextSiblingElement();
            }
        }else if(qe.tagName()=="DeviceWarn"){
          QDomElement child=qe.firstChildElement();
          int index;
          while(!child.isNull()){
              if(child.isElement()){
                  if(child.tagName()=="ChannelId"){
                      if(child.text().toInt()>8 || child.text().toInt()<1){
                          break;
                        }else{
                          index=child.text().toInt();
                        }
                    }else if(child.tagName()=="Open"){
                      QString name="checkBox_deviceWarn_open_"+QString::number(index);
                      QCheckBox *box=(QCheckBox *)widgetMap.find(name).value();
                      if(child.text().toInt()!=0){
                          box->setChecked(true);
                        }else{
                          box->setChecked(false);
                        }
                    }else if(child.tagName()=="WarnOrder"){
                      QString name="checkBox_deviceWarn_warnOrder_"+QString::number(index);
                      QCheckBox *box=(QCheckBox *)widgetMap.find(name).value();
                      if(child.text().toInt()!=0){
                          box->setChecked(true);
                        }else{
                          box->setChecked(false);
                        }
                    }else if(child.tagName()=="FunctionType"){
                      QString name="comboBox_deviceWarn_funcModel_"+QString::number(index);
                      QComboBox *box=(QComboBox *)widgetMap.find(name).value();
                      setComboValue(qe.attribute("method"),child.text(),box);
                    }
                }
              child=child.nextSiblingElement();
            }
        }else if(qe.tagName()=="Code"){
        }
    }else if(e.tagName()=="Picture"){
      //PicManager
      QDomElement qe=e.firstChildElement();

      if(qe.tagName()=="PicManager"){
          QDomElement child=qe.firstChildElement();
          while(!child.isNull()){
              if(child.tagName()=="FindType"){
                  setComboValue(qe.attribute("method"),child.text(),ui->comboBox_picManager_findType);
                }else if(child.tagName()=="ChannelId"){
                  setComboValue(qe.attribute("method"),child.text(), ui->comboBox_picManager_channelId);
                }else if(child.tagName()=="StartTime"){
                  ui->lineEdit_picManager_open->setText(child.text());
                }else if(child.tagName()=="EndTime"){
                  ui->lineEdit_picManager_end->setText(child.text());
                }else if(child.tagName()=="FileName"){
                  QStringList fileList=child.text().split(":");
                  for(int i=0;i<fileList.size();++i){
                      dialog_null_picManagerSubDialog    *log=new dialog_null_picManagerSubDialog(ui->listWidget_picManager_list);
                      log->getLabel()->setText(fileList.at(i));
                      QListWidgetItem *item=new QListWidgetItem(ui->listWidget_picManager_list);
                      ui->listWidget_picManager_list->setItemWidget(item,log);
                      item->setSizeHint (QSize(log->rect().width(),log->rect().height()));
                    }
                }else if(child.tagName()=="BackupFileName"){
                }else if(qe.tagName()=="Code"){

                }
              child=child.nextSiblingElement();
            }
        }
    }else if(e.tagName()=="System"){
      QDomElement qe=e.firstChildElement();
      if(qe.tagName()=="Log"){
          QDomElement child=qe.firstChildElement();
          while(!child.isNull()){
              if(child.tagName()=="Type"){
                  setComboValue(qe.attribute("method"),child.text(), ui->comboBox_log_type);
                }else if(child.tagName()=="All"){
                  ui->lineEdit_log_date->setText(child.text());
                }else if(child.tagName()=="SelectDate"){
                  QDomElement subChild=child.firstChildElement();
                  while(!subChild.isNull()){
                      if(subChild.tagName()=="Open"){
                          if(child.text().toInt()!=0){
                              ui->checkBox_log_date->setChecked(true);
                              ui->lineEdit_log_date->setVisible(true);
                            }else{
                              ui->checkBox_log_date->setChecked(false);
                              ui->lineEdit_log_date->setVisible(false);
                            }
                        }else if(subChild.tagName()=="Date"){
                          ui->lineEdit_log_date->setText(subChild.text());
                        }
                      subChild=subChild.nextSiblingElement();
                    }
                }else if(child.tagName()=="LogFile"){
                  QStringList list=child.text().split(":");
                  for(int i=0;i<list.size();++i){
                      QStringList list1=list.at(i).split("-");
                      if(list1.size()!=2){
                          continue;
                        }
                      dialog_null_logSetSubDialog *log=new dialog_null_logSetSubDialog(ui->listWidget_log_list);
                      log->getTime()->setText(list1.at(0));
                      log->getEvent()->setText(list1.at(1));
                      QListWidgetItem *item=new QListWidgetItem(ui->listWidget_log_list);
                      ui->listWidget_log_list->setItemWidget(item,log);
                      item->setSizeHint (QSize(log->rect().width(),log->rect().height()));
                    }
                }
              child=child.nextSiblingElement();
            }
        }else if(qe.tagName()=="Maintain"){
          QDomElement child=qe.firstChildElement();
          while(!child.isNull()){
              if(child.tagName()=="Restart"){
                  QDomElement subChild=child.firstChildElement();
                  while(!subChild.isNull()){
                      if(subChild.tagName()=="Open"){
                          if(subChild.text().toInt()!=0){
                              ui->checkBox_weihu_restart->setChecked(true);
                              ui->lineEdit_weihu_restart->setVisible(true);
                            }else{
                              ui->checkBox_weihu_restart->setChecked(false);
                              ui->lineEdit_weihu_restart->setVisible(false);
                            }
                        }else if(subChild.tagName()=="Time"){
                          ui->lineEdit_weihu_restart->setText(subChild.text());
                        }
                      subChild=subChild.nextSiblingElement();
                    }
                }else if(child.tagName()=="Upgrade"){
                  setComboValue(qe.attribute("method"),child.text(), ui->comboBox_weihu_updateType);
                }else if(child.tagName()=="server"){
                  setComboValue(qe.attribute("method"),child.text(), ui->comboBox_weihu_server);
                }
              child=child.nextSiblingElement();
            }
        }else if(qe.tagName()=="info"){
          QDomElement child=qe.firstChildElement();
          while(!child.isNull()){
              if(child.tagName()=="ProductType"){
                  ui->label_info_productTypeValue->setText(child.text());
                }else if(child.tagName()=="DeviceType"){
                  ui->label_info_deviceTypeValue->setText(child.text());
                }else if(child.tagName()=="ProductNumber"){
                  ui->label_info_serialNumbersValue->setText(child.text());
                }else if(child.tagName()=="SoftwareVersion"){
                  ui->label_info_softVersionValue->setText(child.text());
                }else if(child.tagName()=="HarderVersion"){
                  ui->label_info_hardwareVersionValue->setText(child.text());
                }else if(child.tagName()=="SoftwareReleaseTime"){
                  ui->label_info_releaseTimeValue->setText(child.text());
                }else if(qe.tagName()=="Code"){

                }
              child=child.nextSiblingElement();
            }
        }
    }else if(e.tagName()=="Other"){
      QDomElement qe=e.firstChildElement();
      if(qe.tagName()=="MemoryDevice"){
          QDomElement child=qe.firstChildElement();
          while(!child.isNull()){
              if(child.tagName()=="FormatSize"){
                  setComboValue(qe.attribute("method"),child.text(), ui->comboBox_memory_diskSize);
                }else if(child.tagName()=="HardDiakInfo"){
                  QStringList list=child.text().split(":");
                  for(int i=0;i<list.size();++i){
                      QStringList list1=list.at(i).split("-");
                      if(list.size()!=4){
                          continue;
                        }
                      dialog_null_memoryDeviceSetSubDialog *log=new dialog_null_memoryDeviceSetSubDialog(ui->listWidget_memory_list);
                      log->getName()->setText(list1.at(0));
                      log->getSize()->setText(list1.at(1));
                      log->getUsed()->setText(list1.at(2));
                      log->getStatus()->setText(list1.at(3));
                      QListWidgetItem *item=new QListWidgetItem(ui->listWidget_memory_list);
                      ui->listWidget_memory_list->setItemWidget(item,log);
                      item->setSizeHint (QSize(log->rect().width(),log->rect().height()));
                    }
                }
              child=child.nextSiblingElement();
            }
        }else if(qe.tagName()=="YunTaiSet"){
          /*<ChannelId></ChannelId>
             <YunTaiAddr></YunTaiAddr>
             <Protocol></Protocol>
             <BaudRate></BaudRate>
             <RLExchange></RLExchange>
             <UDExchange></UDExchange>
            <ApertureExchange></ApertureExchange>
            <ZoomExchange></ZoomExchange>
            <ZoomedExchange></ZoomedExchange>*/
          QDomElement child=qe.firstChildElement();
          while(!child.isNull()){
              if(child.tagName()=="ChannelId"){
                  setComboValue(qe.attribute("method"),child.text(),  ui->comboBox_yuntai_channelId);
                }else if(child.tagName()=="YunTaiAddr"){
                  setComboValue(qe.attribute("method"),child.text(),  ui->comboBox_yuntai_addr);
                }else if(child.tagName()=="Protocol"){
                  setComboValue(qe.attribute("method"),child.text(),  ui->comboBox_yuntai_protocol);
                }else if(child.tagName()=="BaudRate"){
                  setComboValue(qe.attribute("method"),child.text(),  ui->comboBox_yuntai_baudRate);
                }else if(child.tagName()=="RLExchange"){
                  if(child.text().toInt()!=0){
                      ui->checkBox_yuntai_leftRightExchange->setChecked(true);
                    }else{
                      ui->checkBox_yuntai_leftRightExchange->setChecked(false);
                    }
                }else if(child.tagName()=="UDExchange"){
                  if(child.text().toInt()!=0){
                      ui->checkBox_yuntai_upDownExchange->setChecked(true);
                    }else{
                      ui->checkBox_yuntai_upDownExchange->setChecked(true);
                    }
                }else if(child.tagName()=="ApertureExchange"){
                  if(child.text().toInt()!=0){
                      ui->checkBox_yuntai_stopExchange->setChecked(true);
                    }else{
                      ui->checkBox_yuntai_stopExchange->setChecked(true);
                    }
                }else if(child.tagName()=="ZoomExchange"){
                  if(child.text().toInt()!=0){
                      ui->checkBox_yuntai_zoomExchange->setChecked(true);
                    }else{
                      ui->checkBox_yuntai_zoomExchange->setChecked(true);
                    }
                }else if(child.tagName()=="ZoomedExchange"){
                  if(child.text().toInt()!=0){
                      ui->checkBox_yuntai_zoomedExchange->setChecked(true);
                    }else{
                      ui->checkBox_yuntai_zoomedExchange->setChecked(true);
                    }
                }
              child=child.nextSiblingElement();
            }
        }else if(qe.tagName()=="Code"){
        }
    }
}
void dialog_null_systemSetDialog::getSender()
{

  if(!oldName.isEmpty()){
      if(oldName==ui->pushButton_general_subGeneral->objectName()){
          generalSubGeneral();
        }else if(oldName==ui->pushButton_general_time->objectName()){
          generalTime();
        }else if(oldName==ui->pushButton_general_user->objectName()){
          generalUser();
        }else if(oldName==ui->pushButton_channel_preview->objectName()){
          channelPreview();
        }else if(oldName==ui->pushButton_channel_stream->objectName()){
          channelStream();
        }else if(oldName==ui->pushButton_channel_videoSet->objectName()){
          channelVideoSet();
        }else if(oldName==ui->pushButton_channel_audioSet->objectName()){
          channelAudioSet();
        }else if(oldName==ui->pushButton_network_wifi->objectName()){
          networkWifi();
        }else if(oldName==ui->pushButton_network_service->objectName()){
          networkService();
        }else if(oldName==ui->pushButton_network_wireless->objectName()){
          networkWireless();
        }else if(oldName==ui->pushButton_warn_email->objectName()){
          warnEmail();
        }else if(oldName==ui->pushButton_warn_moveTest->objectName()){
          warnMoveTest();
        }else if(oldName==ui->pushButton_warn_videoLost->objectName()){
          warnVideoLost();
        }else if(oldName==ui->pushButton_warn_deviceWarn->objectName()){
          warnDeviceWarn();
        }else if(oldName==ui->pushButton_picManager_picManager->objectName()){
          picManagerPicManager();
        }else if(oldName==ui->pushButton_system_log->objectName()){
          systemLog();
        }else if(oldName==ui->pushButton_system_weihu->objectName()){
          systemWeihu();
        }else if(oldName==ui->pushButton_system_info->objectName()){
          systemInfo();
        }else if(oldName==ui->pushButton_other_memory->objectName()){
          otherMemory();
        }else if(oldName==ui->pushButton_other_yuntai->objectName()){
          otherYuntai();
        }
    }

  senderName.clear();
  QObject *object=QObject::sender();
  senderName=object->objectName();

  if(senderName==ui->pushButton_general_subGeneral->objectName()){
      generalSubGeneral();
    }else if(senderName==ui->pushButton_general_time->objectName()){
      generalTime();
    }else if(senderName==ui->pushButton_general_user->objectName()){
      generalUser();
    }else if(senderName==ui->pushButton_channel_preview->objectName()){
      channelPreview();
    }else if(senderName==ui->pushButton_channel_stream->objectName()){
      channelStream();
    }else if(senderName==ui->pushButton_channel_videoSet->objectName()){
      channelVideoSet();
    }else if(senderName==ui->pushButton_channel_audioSet->objectName()){
      channelAudioSet();
    }else if(senderName==ui->pushButton_network_wifi->objectName()){
      networkWifi();
    }else if(senderName==ui->pushButton_network_service->objectName()){
      networkService();
    }else if(senderName==ui->pushButton_network_wireless->objectName()){
      networkWireless();
    }else if(senderName==ui->pushButton_warn_email->objectName()){
      warnEmail();
    }else if(senderName==ui->pushButton_warn_moveTest->objectName()){
      warnMoveTest();
    }else if(senderName==ui->pushButton_warn_videoLost->objectName()){
      warnVideoLost();
    }else if(senderName==ui->pushButton_warn_deviceWarn->objectName()){
      warnDeviceWarn();
    }else if(senderName==ui->pushButton_picManager_picManager->objectName()){
      picManagerPicManager();
    }else if(senderName==ui->pushButton_system_log->objectName()){
      systemLog();
    }else if(senderName==ui->pushButton_system_weihu->objectName()){
      systemWeihu();
    }else if(senderName==ui->pushButton_system_info->objectName()){
      systemInfo();
    }else if(senderName==ui->pushButton_other_memory->objectName()){
      otherMemory();
    }else if(senderName==ui->pushButton_other_yuntai->objectName()){
      otherYuntai();
    }
}
void dialog_null_systemSetDialog::setComboValue(QString method,QString text,QComboBox *combo)
{
  QStringList list=text.split(":");
  if(method=="Get"){
      for(int i=0;i<list.size();++i){
          combo->addItem(list.at(i));
        }
      combo->setCurrentIndex(0);
    }else if(method=="Set"){
      for(int i=0;i<combo->count();++i){
          combo->setCurrentIndex(i);
          if(text==combo->currentText()){
              combo->setCurrentIndex(i);
              break;
            }
        }
    }
}
void dialog_null_systemSetDialog::recvMessageFromProxy(QString message)
{
  // debug("%s\");
  startXml(message);
}
void dialog_null_systemSetDialog::testFile()
{
  QObjectList list=this->children();

  for(int a=0;a<list.size();++a){
      widgetMap.insert(list.at(a)->objectName(),list.at(a));
      //  createFunctionAndConnection(list.at(a)->objectName());
      QObjectList list0=list.at(a)->children();
      for(int b=0;b<list0.size();++b){
          widgetMap.insert(list0.at(b)->objectName(),list0.at(b));

          // createFunctionAndConnection(list0.at(b)->objectName());
          QObjectList list1=list0.at(b)->children();
          for(int c=0;c<list1.size();++c){
              widgetMap.insert(list1.at(c)->objectName(),list1.at(c));

              //  createFunctionAndConnection(list1.at(c)->objectName());
              QObjectList list2=list1.at(c)->children();
              for(int d=0;d<list2.size();++d){
                  widgetMap.insert(list2.at(d)->objectName(),list2.at(d));
                  // createFunctionAndConnection(list2.at(d)->objectName());
                  QObjectList list3=list2.at(d)->children();
                  for(int e=0;e<list3.size();++e){
                      widgetMap.insert(list3.at(e)->objectName(),list3.at(e));
                      //  createFunctionAndConnection(list3.at(e)->objectName());
                      QObjectList list4=list3.at(e)->children();
                      for(int f=0;f<list4.size();++f){
                          widgetMap.insert(list4.at(f)->objectName(),list4.at(f));
                          //   createFunctionAndConnection(list4.at(f)->objectName());
                        }
                    }
                }
            }
        }
    }

  return;
}
void dialog_null_systemSetDialog::createFunctionAndConnection(QString objectName)
{

  QString functionName;
  functionName.clear();

  QStringList stringList=objectName.split("_");
  if(stringList.size()<3){
      return;
    }
  if(stringList.size()==3){

      functionName.append(stringList.at(0));
      QString fatherName=stringList.at(1);
      QString selfName=stringList.at(2);

      QChar fUp=stringList.at(1).at(0).toUpper();
      QChar sUp=stringList.at(2).at(0).toUpper();

      fatherName.replace(0,1,fUp);
      selfName.replace(0,1,sUp);

      functionName.append(fatherName);
      functionName.append(selfName);
    }
  if(stringList.size()==4){
      functionName.append(stringList.at(0));

      QString fatherName=stringList.at(1);
      QString selfName=stringList.at(2);
      QString id=stringList.at(3);

      QChar fUp=stringList.at(1).at(0).toUpper();
      QChar sUp=stringList.at(2).at(0).toUpper();


      fatherName.replace(0,1,fUp);
      selfName.replace(0,1,sUp);

      functionName.append(fatherName);
      functionName.append(selfName);
      functionName.append(id);
    }

  if(stringList.at(0)=="comboBox"){

      QString functionDeclaration;
      functionDeclaration.append("void ");
      functionDeclaration.append(functionName);
      functionDeclaration.append("(int);");

      QString functionDefinition;
      functionDefinition.append("void dialog_null_systemSetDialog::");
      functionDefinition.append(functionName);
      functionDefinition.append("(int index)");
      functionDefinition.append("{}");

      /*创建连接关系*/
      QString connection;
      connection.append("QObject::connect(ui->");
      connection.append(objectName.toStdString().c_str());
      connection.append(",SIGNAL(activated(int)),this,SLOT(");
      connection.append(functionName.toStdString().c_str());
      connection.append("(int)));");

      //debug("%s\n",functionDeclaration.toStdString().c_str());
      //debug("%s\n",functionDefinition.toStdString().c_str());
      //debug("%s\n",connection.toStdString().c_str());
    }
  if(stringList.at(0)=="pushButton")
    {
      QString functionDeclaration;
      functionDeclaration.append("void ");
      functionDeclaration.append(functionName);
      functionDeclaration.append("();");

      QString functionDefinition;
      functionDefinition.append("void dialog_null_systemSetDialog::");
      functionDefinition.append(functionName);
      functionDefinition.append("()");
      functionDefinition.append("{}");

      /*创建连接关系*/
      QString connection;
      connection.append("QObject::connect(ui->");
      connection.append(objectName.toStdString().c_str());
      connection.append(",SIGNAL(clicked()),this,SLOT(");
      connection.append(functionName.toStdString().c_str());
      connection.append("()));");

      //debug("%s\n",functionDeclaration.toStdString().c_str());
      //debug("%s\n",functionDefinition.toStdString().c_str());
      //debug("%s\n",connection.toStdString().c_str());
    }
  if(stringList.at(0)=="checkBox"){
      QString functionDeclaration;
      functionDeclaration.append("void ");
      functionDeclaration.append(functionName);
      functionDeclaration.append("(int);");

      QString functionDefinition;
      functionDefinition.append("void dialog_null_systemSetDialog::");
      functionDefinition.append(functionName);
      functionDefinition.append("(int flags)");
      functionDefinition.append("{}");

      /*创建连接关系*/
      QString connection;
      connection.append("QObject::connect(ui->");
      connection.append(objectName.toStdString().c_str());
      connection.append(",SIGNAL(stateChanged(int)),this,SLOT(");
      connection.append(functionName.toStdString().c_str());
      connection.append("(int)));");
      //debug("%s\n",functionDeclaration.toStdString().c_str());
      //debug("%s\n",functionDefinition.toStdString().c_str());
      // debug("%s\n",connection.toStdString().c_str());
    }
  if(stringList.at(0)=="lineEdit"){
      QString filter;
      filter.append("ui->");
      filter.append(objectName);
      filter.append("->installEventFilter(this);");
      //debug("%s\n",filter.toStdString().c_str());
      debug("object->objectName()== \"%s\" || \n",objectName.toStdString().c_str());
    }
}
void dialog_null_systemSetDialog::pushButtonOtherYuntai(){}
void dialog_null_systemSetDialog::pushButtonOtherMemory(){}
void dialog_null_systemSetDialog::comboBoxMemoryDiskSize(int index){}
void dialog_null_systemSetDialog::comboBoxYuntaiChannelId(int index){}
void dialog_null_systemSetDialog::comboBoxYuntaiBaudRate(int index){}
void dialog_null_systemSetDialog::comboBoxYuntaiProtocol(int index){}
void dialog_null_systemSetDialog::comboBoxYuntaiAddr(int index){}
void dialog_null_systemSetDialog::checkBoxYuntaiLeftRightExchange(int flags){}
void dialog_null_systemSetDialog::checkBoxYuntaiUpDownExchange(int flags){}
void dialog_null_systemSetDialog::checkBoxYuntaiStopExchange(int flags){}
void dialog_null_systemSetDialog::checkBoxYuntaiZoomedExchange(int flags){}
void dialog_null_systemSetDialog::checkBoxYuntaiZoomExchange(int flags){}
void dialog_null_systemSetDialog::pushButtonOtherUse(){}
void dialog_null_systemSetDialog::pushButtonOtherSure(){}
void dialog_null_systemSetDialog::pushButtonOtherCancel(){}
void dialog_null_systemSetDialog::pushButtonSystemInfo(){}
void dialog_null_systemSetDialog::pushButtonSystemLog(){}
void dialog_null_systemSetDialog::pushButtonSystemWeihu(){}
void dialog_null_systemSetDialog::comboBoxLogType(int index){}
void dialog_null_systemSetDialog::checkBoxLogDate(int flags){}
void dialog_null_systemSetDialog::pushButtonLogBackupToU(){}
void dialog_null_systemSetDialog::checkBoxWeihuRestart(int flags){}
void dialog_null_systemSetDialog::comboBoxWeihuUpdateType(int index){}
void dialog_null_systemSetDialog::comboBoxWeihuServer(int index){}
void dialog_null_systemSetDialog::pushButtonWeihuImportConfig(){}
void dialog_null_systemSetDialog::pushButtonWeihuExportConfig(){}
void dialog_null_systemSetDialog::pushButtonWeihuUpdate(){}
void dialog_null_systemSetDialog::pushButtonWeihuRegister(){}
void dialog_null_systemSetDialog::pushButtonWeihuFormatAll(){}
void dialog_null_systemSetDialog::pushButtonSystemSure(){}
void dialog_null_systemSetDialog::pushButtonSystemUse(){}
void dialog_null_systemSetDialog::pushButtonSystemCancel(){}
void dialog_null_systemSetDialog::pushButtonPicManagerPicManager(){}
void dialog_null_systemSetDialog::comboBoxPicManagerFindType(int index){}
void dialog_null_systemSetDialog::comboBoxPicManagerChannelId(int index){}
void dialog_null_systemSetDialog::pushButtonPicManagerSelectAll(){}
void dialog_null_systemSetDialog::pushButtonPicManagerDel(){}
void dialog_null_systemSetDialog::pushButtonPicManagerBackupToU(){}
void dialog_null_systemSetDialog::pushButtonPicManagerFind(){}
void dialog_null_systemSetDialog::pushButtonPicManagerSure(){}
void dialog_null_systemSetDialog::pushButtonPicManagerUse(){}
void dialog_null_systemSetDialog::pushButtonPicManagerCancel(){}
void dialog_null_systemSetDialog::pushButtonWarnDeviceWarn(){}
void dialog_null_systemSetDialog::pushButtonWarnVideoLost(){}
void dialog_null_systemSetDialog::pushButtonWarnMoveTest(){}
void dialog_null_systemSetDialog::pushButtonWarnEmail(){}
void dialog_null_systemSetDialog::pushButtonVideoLostSelectAll(){}
void dialog_null_systemSetDialog::pushButtonVideoLostDefault(){}
void dialog_null_systemSetDialog::pushButtonVideoLostWarnOut1(){}
void dialog_null_systemSetDialog::checkBoxVideoLostOpen1(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoLostWarnOrder1(int flags){}
void dialog_null_systemSetDialog::pushButtonVideoLostWarnOut2(){}
void dialog_null_systemSetDialog::checkBoxVideoLostOpen2(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoLostWarnOrder2(int flags){}
void dialog_null_systemSetDialog::pushButtonVideoLostWarnOut3(){}
void dialog_null_systemSetDialog::pushButtonVideoLostWarnOut4(){}
void dialog_null_systemSetDialog::checkBoxVideoLostWarnOrder4(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoLostOpen4(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoLostOpen3(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoLostWarnOrder3(int flags){}
void dialog_null_systemSetDialog::pushButtonVideoLostWarnOut5(){}
void dialog_null_systemSetDialog::pushButtonVideoLostWarnOut6(){}
void dialog_null_systemSetDialog::checkBoxVideoLostWarnOrder6(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoLostOpen6(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoLostOpen5(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoLostWarnOrder5(int flags){}
void dialog_null_systemSetDialog::pushButtonVideoLostWarnOut7(){}
void dialog_null_systemSetDialog::pushButtonVideoLostWarnOut8(){}
void dialog_null_systemSetDialog::checkBoxVideoLostWarnOrder8(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoLostOpen8(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoLostOpen7(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoLostWarnOrder7(int flags){}
void dialog_null_systemSetDialog::pushButtonMoveTestSelectAll(){}
void dialog_null_systemSetDialog::pushButtonMoveTestDefault(){}
void dialog_null_systemSetDialog::checkBoxMoveTestOpen1(int flags){}
void dialog_null_systemSetDialog::checkBoxMoveTestWarnOrder1(int flags){}
void dialog_null_systemSetDialog::pushButtonMoveTestAreaSet1(){}
void dialog_null_systemSetDialog::pushButtonMoveTestWarnOut1(){}
void dialog_null_systemSetDialog::checkBoxMoveTestWarnOrder2(int flags){}
void dialog_null_systemSetDialog::checkBoxMoveTestOpen2(int flags){}
void dialog_null_systemSetDialog::pushButtonMoveTestAreaSet2(){}
void dialog_null_systemSetDialog::pushButtonMoveTestWarnOut2(){}
void dialog_null_systemSetDialog::checkBoxMoveTestWarnOrder3(int flags){}
void dialog_null_systemSetDialog::checkBoxMoveTestOpen3(int flags){}
void dialog_null_systemSetDialog::pushButtonMoveTestWarnOut3(){}
void dialog_null_systemSetDialog::pushButtonMoveTestAreaSet3(){}
void dialog_null_systemSetDialog::checkBoxMoveTestWarnOrder4(int flags){}
void dialog_null_systemSetDialog::checkBoxMoveTestOpen4(int flags){}
void dialog_null_systemSetDialog::pushButtonMoveTestWarnOut4(){}
void dialog_null_systemSetDialog::pushButtonMoveTestAreaSet4(){}
void dialog_null_systemSetDialog::checkBoxMoveTestWarnOrder5(int flags){}
void dialog_null_systemSetDialog::checkBoxMoveTestOpen5(int flags){}
void dialog_null_systemSetDialog::pushButtonMoveTestWarnOut5(){}
void dialog_null_systemSetDialog::pushButtonMoveTestAreaSet5(){}
void dialog_null_systemSetDialog::checkBoxMoveTestWarnOrder6(int flags){}
void dialog_null_systemSetDialog::checkBoxMoveTestOpen6(int flags){}
void dialog_null_systemSetDialog::pushButtonMoveTestWarnOut6(){}
void dialog_null_systemSetDialog::pushButtonMoveTestAreaSet6(){}
void dialog_null_systemSetDialog::checkBoxMoveTestWarnOrder7(int flags){}
void dialog_null_systemSetDialog::checkBoxMoveTestOpen7(int flags){}
void dialog_null_systemSetDialog::pushButtonMoveTestWarnOut7(){}
void dialog_null_systemSetDialog::pushButtonMoveTestAreaSet7(){}
void dialog_null_systemSetDialog::checkBoxMoveTestWarnOrder8(int flags){}
void dialog_null_systemSetDialog::checkBoxMoveTestOpen8(int flags){}
void dialog_null_systemSetDialog::pushButtonMoveTestWarnOut8(){}
void dialog_null_systemSetDialog::pushButtonMoveTestAreaSet8(){}
void dialog_null_systemSetDialog::pushButtonEmailAddList(){}
void dialog_null_systemSetDialog::pushButtonEmailEditList(){}
void dialog_null_systemSetDialog::pushButtonEmailDelList(){}
void dialog_null_systemSetDialog::pushButtonEmailServerPort(){}
void dialog_null_systemSetDialog::comboBoxEmailServer(int index){}
void dialog_null_systemSetDialog::comboBoxEmailSafeType(int index){}
void dialog_null_systemSetDialog::pushButtonEmailTest(){}
void dialog_null_systemSetDialog::pushButtonDeviceWarnSelectAll(){}
void dialog_null_systemSetDialog::pushButtonDeviceWarnDefault(){}
void dialog_null_systemSetDialog::checkBoxDeviceWarnOpen1(int flags){}
void dialog_null_systemSetDialog::checkBoxDeviceWarnWarnOrder1(int flags){}
void dialog_null_systemSetDialog::pushButtonDeviceWarnWarnOut1(){}
void dialog_null_systemSetDialog::comboBoxDeviceWarnFuncModel1(int index){}
void dialog_null_systemSetDialog::checkBoxDeviceWarnOpen2(int flags){}
void dialog_null_systemSetDialog::checkBoxDeviceWarnWarnOrder2(int flags){}
void dialog_null_systemSetDialog::comboBoxDeviceWarnFuncModel2(int index){}
void dialog_null_systemSetDialog::pushButtonDeviceWarnWarnOut2(){}
void dialog_null_systemSetDialog::checkBoxDeviceWarnWarnOrder4(int flags){}
void dialog_null_systemSetDialog::checkBoxDeviceWarnOpen4(int flags){}
void dialog_null_systemSetDialog::checkBoxDeviceWarnOpen3(int flags){}
void dialog_null_systemSetDialog::checkBoxDeviceWarnWarnOrder3(int flags){}
void dialog_null_systemSetDialog::comboBoxDeviceWarnFuncModel4(int index){}
void dialog_null_systemSetDialog::comboBoxDeviceWarnFuncModel3(int index){}
void dialog_null_systemSetDialog::pushButtonDeviceWarnWarnOut3(){}
void dialog_null_systemSetDialog::pushButtonDeviceWarnWarnOut4(){}
void dialog_null_systemSetDialog::checkBoxDeviceWarnWarnOrder6(int flags){}
void dialog_null_systemSetDialog::checkBoxDeviceWarnOpen6(int flags){}
void dialog_null_systemSetDialog::checkBoxDeviceWarnOpen5(int flags){}
void dialog_null_systemSetDialog::checkBoxDeviceWarnWarnOrder5(int flags){}
void dialog_null_systemSetDialog::comboBoxDeviceWarnFuncModel6(int index){}
void dialog_null_systemSetDialog::comboBoxDeviceWarnFuncModel5(int index){}
void dialog_null_systemSetDialog::pushButtonDeviceWarnWarnOut5(){}
void dialog_null_systemSetDialog::pushButtonDeviceWarnWarnOut6(){}
void dialog_null_systemSetDialog::checkBoxDeviceWarnWarnOrder8(int flags){}
void dialog_null_systemSetDialog::checkBoxDeviceWarnOpen8(int flags){}
void dialog_null_systemSetDialog::checkBoxDeviceWarnOpen7(int flags){}
void dialog_null_systemSetDialog::checkBoxDeviceWarnWarnOrder7(int flags){}
void dialog_null_systemSetDialog::comboBoxDeviceWarnFuncModel8(int index){}
void dialog_null_systemSetDialog::comboBoxDeviceWarnFuncModel7(int index){}
void dialog_null_systemSetDialog::pushButtonDeviceWarnWarnOut7(){}
void dialog_null_systemSetDialog::pushButtonDeviceWarnWarnOut8(){}
void dialog_null_systemSetDialog::pushButtonWarnSure(){}
void dialog_null_systemSetDialog::pushButtonWarnUse(){}
void dialog_null_systemSetDialog::pushButtonWarnCancel(){}
void dialog_null_systemSetDialog::pushButtonNetworkService(){}
void dialog_null_systemSetDialog::pushButtonNetworkWifi(){}
void dialog_null_systemSetDialog::pushButtonNetworkWireless(){}
void dialog_null_systemSetDialog::checkBoxWirelessDhcp(int flags){}
void dialog_null_systemSetDialog::pushButtonWifiADSLConnect(){}
void dialog_null_systemSetDialog::pushButtonWifiADSLDisconnect(){}
void dialog_null_systemSetDialog::pushButtonWifiCardDisconnect(){}
void dialog_null_systemSetDialog::pushButtonWifiCardConnect(){}
void dialog_null_systemSetDialog::pushButtonNetworkCancel(){}
void dialog_null_systemSetDialog::pushButtonNetworkSure(){}
void dialog_null_systemSetDialog::pushButtonNetworkUse(){}
void dialog_null_systemSetDialog::pushButtonChannelStream(){}
void dialog_null_systemSetDialog::pushButtonChannelAudioSet(){}
void dialog_null_systemSetDialog::pushButtonChannelVideoSet(){}
void dialog_null_systemSetDialog::pushButtonChannelPreview(){}
void dialog_null_systemSetDialog::pushButtonVideoSetVideoCopy(){}
void dialog_null_systemSetDialog::pushButtonVideoSetDefault(){}
void dialog_null_systemSetDialog::checkBoxVideoSetMyselfVideo1(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoSetTimingVideoTwo1(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoSetTimingVideoOne1(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoSetMyselfVideo2(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoSetTimingVideoOne2(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoSetTimingVideoTwo2(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoSetMyselfVideo3(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoSetMyselfVideo4(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoSetTimingVideoOne3(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoSetTimingVideoTwo3(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoSetTimingVideoTwo4(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoSetTimingVideoOne4(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoSetMyselfVideo5(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoSetMyselfVideo6(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoSetTimingVideoOne5(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoSetTimingVideoTwo5(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoSetTimingVideoTwo6(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoSetTimingVideoOne6(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoSetMyselfVideo7(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoSetMyselfVideo8(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoSetTimingVideoOne7(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoSetTimingVideoTwo7(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoSetTimingVideoTwo8(int flags){}
void dialog_null_systemSetDialog::checkBoxVideoSetTimingVideoOne8(int flags){}
void dialog_null_systemSetDialog::comboBoxStreamMinorFenBianLv1(int index){}
void dialog_null_systemSetDialog::comboBoxStreamFenbialv1(int index){}
void dialog_null_systemSetDialog::comboBoxStreamLiuChangDu1(int index){}
void dialog_null_systemSetDialog::comboBoxStreamLiuChangDu2(int index){}
void dialog_null_systemSetDialog::comboBoxStreamMinorFenBianLv2(int index){}
void dialog_null_systemSetDialog::comboBoxStreamFenbialv2(int index){}
void dialog_null_systemSetDialog::comboBoxStreamFenbialv4(int index){}
void dialog_null_systemSetDialog::comboBoxStreamLiuChangDu4(int index){}
void dialog_null_systemSetDialog::comboBoxStreamLiuChangDu3(int index){}
void dialog_null_systemSetDialog::comboBoxStreamMinorFenBianLv4(int index){}
void dialog_null_systemSetDialog::comboBoxStreamMinorFenBianLv3(int index){}
void dialog_null_systemSetDialog::comboBoxStreamFenbialv3(int index){}
void dialog_null_systemSetDialog::comboBoxStreamFenbialv6(int index){}
void dialog_null_systemSetDialog::comboBoxStreamLiuChangDu6(int index){}
void dialog_null_systemSetDialog::comboBoxStreamLiuChangDu5(int index){}
void dialog_null_systemSetDialog::comboBoxStreamMinorFenBianLv6(int index){}
void dialog_null_systemSetDialog::comboBoxStreamMinorFenBianLv5(int index){}
void dialog_null_systemSetDialog::comboBoxStreamFenbialv5(int index){}
void dialog_null_systemSetDialog::comboBoxStreamFenbialv8(int index){}
void dialog_null_systemSetDialog::comboBoxStreamLiuChangDu8(int index){}
void dialog_null_systemSetDialog::comboBoxStreamLiuChangDu7(int index){}
void dialog_null_systemSetDialog::comboBoxStreamMinorFenBianLv8(int index){}
void dialog_null_systemSetDialog::comboBoxStreamMinorFenBianLv7(int index){}
void dialog_null_systemSetDialog::comboBoxStreamFenbialv7(int index){}
void dialog_null_systemSetDialog::pushButtonPreviewCopy(){}
void dialog_null_systemSetDialog::pushButtonPreviewSetDefault(){}
void dialog_null_systemSetDialog::pushButtonPreviewChannelSort(){}
void dialog_null_systemSetDialog::checkBoxPreviewArenHide1(int flags){}
void dialog_null_systemSetDialog::pushButtonPreviewAreaHideSet1(){}
void dialog_null_systemSetDialog::checkBoxPreviewHide1(int flags){}
void dialog_null_systemSetDialog::comboBoxPreviewOSD1(int index){}
void dialog_null_systemSetDialog::checkBoxPreviewArenHide2(int flags){}
void dialog_null_systemSetDialog::pushButtonPreviewAreaHideSet2(){}
void dialog_null_systemSetDialog::checkBoxPreviewHide2(int flags){}
void dialog_null_systemSetDialog::comboBoxPreviewOSD2(int index){}
void dialog_null_systemSetDialog::checkBoxPreviewArenHide3(int flags){}
void dialog_null_systemSetDialog::pushButtonPreviewAreaHideSet3(){}
void dialog_null_systemSetDialog::checkBoxPreviewHide3(int flags){}
void dialog_null_systemSetDialog::comboBoxPreviewOSD3(int index){}
void dialog_null_systemSetDialog::checkBoxPreviewArenHide4(int flags){}
void dialog_null_systemSetDialog::pushButtonPreviewAreaHideSet4(){}
void dialog_null_systemSetDialog::checkBoxPreviewHide4(int flags){}
void dialog_null_systemSetDialog::comboBoxPreviewOSD4(int index){}
void dialog_null_systemSetDialog::checkBoxPreviewArenHide5(int flags){}
void dialog_null_systemSetDialog::pushButtonPreviewAreaHideSet5(){}
void dialog_null_systemSetDialog::checkBoxPreviewHide5(int flags){}
void dialog_null_systemSetDialog::comboBoxPreviewOSD5(int index){}
void dialog_null_systemSetDialog::checkBoxPreviewArenHide6(int flags){}
void dialog_null_systemSetDialog::pushButtonPreviewAreaHideSet6(){}
void dialog_null_systemSetDialog::checkBoxPreviewHide6(int flags){}
void dialog_null_systemSetDialog::comboBoxPreviewOSD6(int index){}
void dialog_null_systemSetDialog::checkBoxPreviewArenHide7(int flags){}
void dialog_null_systemSetDialog::pushButtonPreviewAreaHideSet7(){}
void dialog_null_systemSetDialog::checkBoxPreviewHide7(int flags){}
void dialog_null_systemSetDialog::comboBoxPreviewOSD7(int index){}
void dialog_null_systemSetDialog::checkBoxPreviewArenHide8(int flags){}
void dialog_null_systemSetDialog::pushButtonPreviewAreaHideSet8(){}
void dialog_null_systemSetDialog::checkBoxPreviewHide8(int flags){}
void dialog_null_systemSetDialog::comboBoxPreviewOSD8(int index){}
void dialog_null_systemSetDialog::comboBoxAudioSetAudioZoom1(int index){}
void dialog_null_systemSetDialog::checkBoxAudioSetAudio1(int flags){}
void dialog_null_systemSetDialog::comboBoxAudioSetBindVideo1(int index){}
void dialog_null_systemSetDialog::checkBoxAudioSetAudio2(int flags){}
void dialog_null_systemSetDialog::comboBoxAudioSetBindVideo2(int index){}
void dialog_null_systemSetDialog::comboBoxAudioSetAudioZoom2(int index){}
void dialog_null_systemSetDialog::checkBoxAudioSetAudio3(int flags){}
void dialog_null_systemSetDialog::comboBoxAudioSetBindVideo4(int index){}
void dialog_null_systemSetDialog::comboBoxAudioSetAudioZoom4(int index){}
void dialog_null_systemSetDialog::checkBoxAudioSetAudio4(int flags){}
void dialog_null_systemSetDialog::comboBoxAudioSetBindVideo3(int index){}
void dialog_null_systemSetDialog::comboBoxAudioSetAudioZoom3(int index){}
void dialog_null_systemSetDialog::pushButtonChannelSure(){}
void dialog_null_systemSetDialog::pushButtonChannnelUse(){}
void dialog_null_systemSetDialog::pushButtonChannelCancel(){}
void dialog_null_systemSetDialog::pushButtonGeneralSubGeneral(){}
void dialog_null_systemSetDialog::pushButtonGeneralTime(){}
void dialog_null_systemSetDialog::pushButtonGeneralUser(){}
void dialog_null_systemSetDialog::comboBoxUserUserList(int index){}
void dialog_null_systemSetDialog::pushButtonUserUserAdd()
{
  static dialog_null_editUserDialog *editUser=NULL;
  if(editUser==NULL){
      editUser=new dialog_null_editUserDialog(this);
    }
  editUser->setWindowModality(Qt::ApplicationModal);
  editUser->show();


}
void dialog_null_systemSetDialog::pushButtonUserUserEdit(){}
void dialog_null_systemSetDialog::pushButtonUserUserDel(){}
void dialog_null_systemSetDialog::checkBoxSubGeneralGuide(int flags){}
void dialog_null_systemSetDialog::comboBoxSubGeneralTransparent(int index){}
void dialog_null_systemSetDialog::comboBoxSubGeneralVGAResolutionRatio(int index){}
void dialog_null_systemSetDialog::comboBoxSubGeneralSensorData(int index){}
void dialog_null_systemSetDialog::checkBoxTimeAutoGetTime(int flags){}
void dialog_null_systemSetDialog::comboBoxTimeTimeFormat(int index){}
void dialog_null_systemSetDialog::comboBoxTimeTimeArea(int index){}
void dialog_null_systemSetDialog::pushButtonGeneralSure()
{
  pushButtonChannnelUse();
}
void dialog_null_systemSetDialog::pushButtonGeneralCancel(){}
void dialog_null_systemSetDialog::pushButtonGeneralUse()
{

}

void dialog_null_systemSetDialog::curRow(int row)
{
  QListWidgetItem *it= ui->listWidget_email_list->item(row);
  dialog_null_emailSetSubDialog *widget=(dialog_null_emailSetSubDialog *)ui->listWidget_email_list->itemWidget(it);
  //name=widget->getLabe()->text();
}

