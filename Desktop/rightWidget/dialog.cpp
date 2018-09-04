#include "dialog.h"
#include "ui_dialog.h"
#include <QMouseEvent>
#include "proxy.h"
#include <QFile>
#include <QTextStream>
#include "keyboard/softkeyboard.h"
#include "tipdialog.h"
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

dialog_null_systemSetDialog::dialog_null_systemSetDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_systemSetDialog)
{
  ui->setupUi(this);
  this->setWindowFlags(Qt::FramelessWindowHint );
  tDialog= new dialog_null_TipDialog;
  initWidget();

  // testFile();

  // this->setStyleSheet("QLabel{color:#FFFF00;}QPushButton{color:#FFFFFF;}");
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
      if(object->objectName()== "lineEdit_weihu_restart" ||
         object->objectName()== "lineEdit_log_selectAll" ||
         object->objectName()== "lineEdit_log_date" ||
         object->objectName()== "lineEdit_picManager_open" ||
         object->objectName()== "lineEdit_picManager_end" ||
         object->objectName()== "lineEdit_email_serverPort" ||
         object->objectName()== "lineEdit_email_userName" ||
         object->objectName()== "lineEdit_email_userpassword" ||
         object->objectName()== "lineEdit_email_interval" ||
         object->objectName()== "lineEdit_user_autoLoginOut" ||
         object->objectName()== "lineEdit_display_date" ||
         object->objectName()== "lineEdit_display_time" ||
         object->objectName()== "lineEdit_service_webPort" ||
         object->objectName()== "lineEdit_service_videoForwardingAddr" ||
         object->objectName()== "lineEdit_service_videoForwardingPort" ||
         object->objectName()== "lineEdit_service_videoUploadPort" ||
         object->objectName()== "lineEdit_service_videoUploadAddr" ||
         object->objectName()== "lineEdit_service_talkPort" ||
         object->objectName()== "lineEdit_wifi_ADSLName" ||
         object->objectName()== "lineEdit_wifi_ADSLPassword" ||
         object->objectName()== "lineEdit_wifi_cardPassword" ||
         object->objectName()== "lineEdit_wifi_cardName" ||
         object->objectName()== "lineEdit_wireless_ip" ||
         object->objectName()== "lineEdit_wireless_gateway" ||
         object->objectName()== "lineEdit_wireless_dns" ||
         object->objectName()== "lineEdit_wireless_mask" ||
         object->objectName()== "lineEdit_wireless_mac"){
          debug("%s\n","=============");
          QLineEdit *lEdit=dynamic_cast<QLineEdit *>(object);
          SoftKeyboard::instance()->show();
          SoftKeyboard::instance()->setLineText(lEdit);

        }
    }

  return QDialog::eventFilter(object,e);
}
void dialog_null_systemSetDialog::initWidget()
{
  ui->label_systemSetDialog_shutdown->setText("  X");
  ui->label_systemSetDialog_shutdown->installEventFilter(this);

  // ui->label->setObjectName("Title");


  //  ui->label_shutdow->setStyleSheet("color:#FFFF00;");



  Proxy *con1=new Proxy(ui->stackedWidget_general,this);
  QObject::connect(ui->pushButton_general_subGeneral,SIGNAL(clicked()),con1,SLOT(currentIndex0()));
  // debug("<!---- %s ---->\n",ui->pushButton_general->objectName().toStdString().c_str());
  QObject::connect(ui->pushButton_general_time,SIGNAL(clicked()),con1,SLOT(currentIndex1()));
  QObject::connect(ui->pushButton_general_display,SIGNAL(clicked()),con1,SLOT(currentIndex2()));
  QObject::connect(ui->pushButton_general_user,SIGNAL(clicked()),con1,SLOT(currentIndex3()));

  Proxy *con2=new Proxy(ui->stackedWidget_channel,this);
  QObject::connect(ui->pushButton_channel_preview,SIGNAL(clicked()),con2,SLOT(currentIndex0()));
  QObject::connect(ui->pushButton_channel_stream,SIGNAL(clicked()),con2,SLOT(currentIndex1()));
  QObject::connect(ui->pushButton_channel_videoSet,SIGNAL(clicked()),con2,SLOT(currentIndex2()));
  QObject::connect(ui->pushButton_channel_audioSet,SIGNAL(clicked()),con2,SLOT(currentIndex3()));

  Proxy *con3=new Proxy(ui->stackedWidget_network,this);
  QObject::connect(ui->pushButton_network_wifi,SIGNAL(clicked()),con3,SLOT(currentIndex0()));
  QObject::connect(ui->pushButton_network_wireless,SIGNAL(clicked()),con3,SLOT(currentIndex1()));
  QObject::connect(ui->pushButton_network_service,SIGNAL(clicked()),con3,SLOT(currentIndex2()));

  Proxy *con4=new Proxy(ui->stackedWidget_warn,this);
  QObject::connect(ui->pushButton_warn_email,SIGNAL(clicked()),con4,SLOT(currentIndex0()));
  QObject::connect(ui->pushButton_warn_moveTest,SIGNAL(clicked()),con4,SLOT(currentIndex1()));
  QObject::connect(ui->pushButton_warn_videoLost,SIGNAL(clicked()),con4,SLOT(currentIndex2()));
  QObject::connect(ui->pushButton_warn_deviceWarn,SIGNAL(clicked()),con4,SLOT(currentIndex3()));

  Proxy *con5=new Proxy(ui->stackedWidget_picManager,this);
  QObject::connect(ui->pushButton_picManager_picManager,SIGNAL(clicked()),con5,SLOT(currentIndex0()));

  Proxy *con6=new Proxy(ui->stackedWidget_system,this);
  QObject::connect(ui->pushButton_system_log,SIGNAL(clicked()),con6,SLOT(currentIndex0()));
  QObject::connect(ui->pushButton_system_weihu,SIGNAL(clicked()),con6,SLOT(currentIndex1()));
  QObject::connect(ui->pushButton_system_info,SIGNAL(clicked()),con6,SLOT(currentIndex2()));

  Proxy *con7=new Proxy(ui->stackedWidget_other,this);
  QObject::connect(ui->pushButton_other_memory,SIGNAL(clicked()),con7,SLOT(currentIndex0()));
  QObject::connect(ui->pushButton_other_yuntai,SIGNAL(clicked()),con7,SLOT(currentIndex1()));
  QObject::connect(ui->pushButton_other_otherDevice,SIGNAL(clicked()),con7,SLOT(currentIndex2()));


  // QObject::connect(ui->pushButton_other_yuntai,SIGNAL(clicked()),this,SLOT(pushButtonOtherYuntai()));
  // QObject::connect(ui->pushButton_other_otherDevice,SIGNAL(clicked()),this,SLOT(pushButtonOtherOtherDevice()));
  // QObject::connect(ui->pushButton_other_memory,SIGNAL(clicked()),this,SLOT(pushButtonOtherMemory()));
  QObject::connect(ui->comboBox_yuntai_channelId,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxYuntaiChannelId(int)));
  QObject::connect(ui->comboBox_yuntai_baudRate,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxYuntaiBaudRate(int)));
  QObject::connect(ui->comboBox_yuntai_protocol,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxYuntaiProtocol(int)));
  QObject::connect(ui->comboBox_yuntai_addr,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxYuntaiAddr(int)));
  QObject::connect(ui->checkBox_yuntai_leftRightExchange,SIGNAL(stateChanged(int)),this,SLOT(checkBoxYuntaiLeftRightExchange(int)));
  QObject::connect(ui->checkBox_yuntai_upDownExchange,SIGNAL(stateChanged(int)),this,SLOT(checkBoxYuntaiUpDownExchange(int)));
  QObject::connect(ui->checkBox_yuntai_stopExchange,SIGNAL(stateChanged(int)),this,SLOT(checkBoxYuntaiStopExchange(int)));
  QObject::connect(ui->checkBox_yuntai_zoomedExchange,SIGNAL(stateChanged(int)),this,SLOT(checkBoxYuntaiZoomedExchange(int)));
  QObject::connect(ui->checkBox_yuntai_zoomExchange,SIGNAL(stateChanged(int)),this,SLOT(checkBoxYuntaiZoomExchange(int)));
  QObject::connect(ui->comboBox_memory_diskSize,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxMemoryDiskSize(int)));
  QObject::connect(ui->pushButton_other_use,SIGNAL(clicked()),this,SLOT(pushButtonOtherUse()));
  QObject::connect(ui->pushButton_other_sure,SIGNAL(clicked()),this,SLOT(pushButtonOtherSure()));
  QObject::connect(ui->pushButton_other_cancel,SIGNAL(clicked()),this,SLOT(pushButtonOtherCancel()));
  //  QObject::connect(ui->pushButton_system_info,SIGNAL(clicked()),this,SLOT(pushButtonSystemInfo()));
  //  QObject::connect(ui->pushButton_system_log,SIGNAL(clicked()),this,SLOT(pushButtonSystemLog()));
  //  QObject::connect(ui->pushButton_system_weihu,SIGNAL(clicked()),this,SLOT(pushButtonSystemWeihu()));
  QObject::connect(ui->checkBox_weihu_restart,SIGNAL(stateChanged(int)),this,SLOT(checkBoxWeihuRestart(int)));
  QObject::connect(ui->comboBox_weihu_updateType,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxWeihuUpdateType(int)));
  QObject::connect(ui->comboBox_weihu_server,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxWeihuServer(int)));
  QObject::connect(ui->pushButton_weihu_importConfig,SIGNAL(clicked()),this,SLOT(pushButtonWeihuImportConfig()));
  QObject::connect(ui->pushButton_weihu_exportConfig,SIGNAL(clicked()),this,SLOT(pushButtonWeihuExportConfig()));
  QObject::connect(ui->pushButton_weihu_update,SIGNAL(clicked()),this,SLOT(pushButtonWeihuUpdate()));
  QObject::connect(ui->pushButton_weihu_register,SIGNAL(clicked()),this,SLOT(pushButtonWeihuRegister()));
  QObject::connect(ui->pushButton_weihu_formatAll,SIGNAL(clicked()),this,SLOT(pushButtonWeihuFormatAll()));
  QObject::connect(ui->comboBox_log_type,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxLogType(int)));
  QObject::connect(ui->checkBox_log_date,SIGNAL(stateChanged(int)),this,SLOT(checkBoxLogDate(int)));
  QObject::connect(ui->pushButton_log_backupToU,SIGNAL(clicked()),this,SLOT(pushButtonLogBackupToU()));
  QObject::connect(ui->pushButton_system_sure,SIGNAL(clicked()),this,SLOT(pushButtonSystemSure()));
  QObject::connect(ui->pushButton_system_use,SIGNAL(clicked()),this,SLOT(pushButtonSystemUse()));
  QObject::connect(ui->pushButton_system_cancel,SIGNAL(clicked()),this,SLOT(pushButtonSystemCancel()));
  // QObject::connect(ui->pushButton_picManager_picManager,SIGNAL(clicked()),this,SLOT(pushButtonPicManagerPicManager()));
  QObject::connect(ui->comboBox_picManager_findType,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxPicManagerFindType(int)));
  QObject::connect(ui->comboBox_picManager_channelId,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxPicManagerChannelId(int)));
  QObject::connect(ui->pushButton_picManager_selectAll,SIGNAL(clicked()),this,SLOT(pushButtonPicManagerSelectAll()));
  QObject::connect(ui->pushButton_picManager_del,SIGNAL(clicked()),this,SLOT(pushButtonPicManagerDel()));
  QObject::connect(ui->pushButton_picManager_backupToU,SIGNAL(clicked()),this,SLOT(pushButtonPicManagerBackupToU()));
  QObject::connect(ui->pushButton_picManager_find,SIGNAL(clicked()),this,SLOT(pushButtonPicManagerFind()));
  QObject::connect(ui->pushButton_picManager_sure,SIGNAL(clicked()),this,SLOT(pushButtonPicManagerSure()));
  QObject::connect(ui->pushButton_picManager_use,SIGNAL(clicked()),this,SLOT(pushButtonPicManagerUse()));
  QObject::connect(ui->pushButton_picManager_cancel,SIGNAL(clicked()),this,SLOT(pushButtonPicManagerCancel()));
  //QObject::connect(ui->pushButton_warn_deviceWarn,SIGNAL(clicked()),this,SLOT(pushButtonWarnDeviceWarn()));
  //QObject::connect(ui->pushButton_warn_videoLost,SIGNAL(clicked()),this,SLOT(pushButtonWarnVideoLost()));
  // QObject::connect(ui->pushButton_warn_moveTest,SIGNAL(clicked()),this,SLOT(pushButtonWarnMoveTest()));
  // QObject::connect(ui->pushButton_warn_email,SIGNAL(clicked()),this,SLOT(pushButtonWarnEmail()));
  QObject::connect(ui->pushButton_videoLost_selectAll,SIGNAL(clicked()),this,SLOT(pushButtonVideoLostSelectAll()));
  QObject::connect(ui->pushButton_videoLost_default,SIGNAL(clicked()),this,SLOT(pushButtonVideoLostDefault()));
  QObject::connect(ui->pushButton_moveTest_selectAll,SIGNAL(clicked()),this,SLOT(pushButtonMoveTestSelectAll()));
  QObject::connect(ui->pushButton_moveTest_default,SIGNAL(clicked()),this,SLOT(pushButtonMoveTestDefault()));
  QObject::connect(ui->pushButton_email_addList,SIGNAL(clicked()),this,SLOT(pushButtonEmailAddList()));
  QObject::connect(ui->pushButton_email_editList,SIGNAL(clicked()),this,SLOT(pushButtonEmailEditList()));
  QObject::connect(ui->pushButton_email_delList,SIGNAL(clicked()),this,SLOT(pushButtonEmailDelList()));
  QObject::connect(ui->pushButton_email_serverPort,SIGNAL(clicked()),this,SLOT(pushButtonEmailServerPort()));
  QObject::connect(ui->comboBox_email_server,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxEmailServer(int)));
  QObject::connect(ui->comboBox_email_safeType,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxEmailSafeType(int)));
  QObject::connect(ui->pushButton_email_test,SIGNAL(clicked()),this,SLOT(pushButtonEmailTest()));
  QObject::connect(ui->pushButton_deviceWarn_selectAll,SIGNAL(clicked()),this,SLOT(pushButtonDeviceWarnSelectAll()));
  QObject::connect(ui->pushButton_deviceWarn_default,SIGNAL(clicked()),this,SLOT(pushButtonDeviceWarnDefault()));
  QObject::connect(ui->pushButton_warn_sure,SIGNAL(clicked()),this,SLOT(pushButtonWarnSure()));
  QObject::connect(ui->pushButton_warn_use,SIGNAL(clicked()),this,SLOT(pushButtonWarnUse()));
  QObject::connect(ui->pushButton_warn_cancel,SIGNAL(clicked()),this,SLOT(pushButtonWarnCancel()));
  // QObject::connect(ui->pushButton_network_netservice,SIGNAL(clicked()),this,SLOT(pushButtonNetworkNetservice()));
  //QObject::connect(ui->pushButton_network_wifi,SIGNAL(clicked()),this,SLOT(pushButtonNetworkWifi()));
  //QObject::connect(ui->pushButton_network_wireless,SIGNAL(clicked()),this,SLOT(pushButtonNetworkWireless()));
  QObject::connect(ui->checkBox_service_web,SIGNAL(stateChanged(int)),this,SLOT(checkBoxServiceWeb(int)));
  QObject::connect(ui->checkBox_service_talk,SIGNAL(stateChanged(int)),this,SLOT(checkBoxServiceTalk(int)));
  QObject::connect(ui->checkBox_service_streamServer,SIGNAL(stateChanged(int)),this,SLOT(checkBoxServiceStreamServer(int)));
  QObject::connect(ui->pushButton_wifi_ADSLConnect,SIGNAL(clicked()),this,SLOT(pushButtonWifiADSLConnect()));
  QObject::connect(ui->pushButton_wifi_ADSLDisconnect,SIGNAL(clicked()),this,SLOT(pushButtonWifiADSLDisconnect()));
  QObject::connect(ui->pushButton_wifi_cardDisconnect,SIGNAL(clicked()),this,SLOT(pushButtonWifiCardDisconnect()));
  QObject::connect(ui->pushButton_wifi_cardConnect,SIGNAL(clicked()),this,SLOT(pushButtonWifiCardConnect()));
  QObject::connect(ui->checkBox_wireless_dhcp,SIGNAL(stateChanged(int)),this,SLOT(checkBoxWirelessDhcp(int)));
  QObject::connect(ui->pushButton_network_cancel,SIGNAL(clicked()),this,SLOT(pushButtonNetworkCancel()));
  QObject::connect(ui->pushButton_network_sure,SIGNAL(clicked()),this,SLOT(pushButtonNetworkSure()));
  QObject::connect(ui->pushButton_network_use,SIGNAL(clicked()),this,SLOT(pushButtonNetworkUse()));
  //QObject::connect(ui->pushButton_channel_stream,SIGNAL(clicked()),this,SLOT(pushButtonChannelStream()));
  // QObject::connect(ui->pushButton_channel_audio,SIGNAL(clicked()),this,SLOT(pushButtonChannelAudio()));
  //  QObject::connect(ui->pushButton_channel_video,SIGNAL(clicked()),this,SLOT(pushButtonChannelVideo()));
  //QObject::connect(ui->pushButton_channel_preview,SIGNAL(clicked()),this,SLOT(pushButtonChannelPreview()));
  QObject::connect(ui->pushButton_videoSet_videoCopy,SIGNAL(clicked()),this,SLOT(pushButtonVideoSetVideoCopy()));
  QObject::connect(ui->pushButton_videoSet_default,SIGNAL(clicked()),this,SLOT(pushButtonVideoSetDefault()));
  QObject::connect(ui->pushButton_preview_copy,SIGNAL(clicked()),this,SLOT(pushButtonPreviewCopy()));
  QObject::connect(ui->pushButton_preview_setDefault,SIGNAL(clicked()),this,SLOT(pushButtonPreviewSetDefault()));
  QObject::connect(ui->pushButton_preview_channelSort,SIGNAL(clicked()),this,SLOT(pushButtonPreviewChannelSort()));
  QObject::connect(ui->pushButton_channel_sure,SIGNAL(clicked()),this,SLOT(pushButtonChannelSure()));
  QObject::connect(ui->pushButton_channnel_use,SIGNAL(clicked()),this,SLOT(pushButtonChannnelUse()));
  QObject::connect(ui->pushButton_channel_cancel,SIGNAL(clicked()),this,SLOT(pushButtonChannelCancel()));
  // QObject::connect(ui->pushButton_general_general,SIGNAL(clicked()),this,SLOT(pushButtonGeneralGeneral()));
  //QObject::connect(ui->pushButton_general_time,SIGNAL(clicked()),this,SLOT(pushButtonGeneralTime()));
  //QObject::connect(ui->pushButton_general_display,SIGNAL(clicked()),this,SLOT(pushButtonGeneralDisplay()));
  //QObject::connect(ui->pushButton_general_user,SIGNAL(clicked()),this,SLOT(pushButtonGeneralUser()));
  QObject::connect(ui->comboBox_user_userGroup,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxUserUserGroup(int)));
  QObject::connect(ui->comboBox_user_userList,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxUserUserList(int)));
  QObject::connect(ui->pushButton_user_userAdd,SIGNAL(clicked()),this,SLOT(pushButtonUserUserAdd()));
  QObject::connect(ui->pushButton_user_userEdit,SIGNAL(clicked()),this,SLOT(pushButtonUserUserEdit()));
  QObject::connect(ui->pushButton_user_userDel,SIGNAL(clicked()),this,SLOT(pushButtonUserUserDel()));
  QObject::connect(ui->checkBox_display_sensorData,SIGNAL(stateChanged(int)),this,SLOT(checkBoxDisplaySensorData(int)));
  QObject::connect(ui->checkBox_subGeneral_guide,SIGNAL(stateChanged(int)),this,SLOT(checkBoxGeneralGuide(int)));
  QObject::connect(ui->comboBox_subGeneral_language,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxGeneralLanguage(int)));
  QObject::connect(ui->comboBox_subGeneral_videoFormat,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxGeneralVideoFormat(int)));
  QObject::connect(ui->comboBox_subGeneral_videoResolutionRatio,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxGeneralVideoResolutionRatio(int)));
  QObject::connect(ui->comboBox_subGeneral_transparent,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxGeneralTransparent(int)));
  QObject::connect(ui->comboBox_subGeneral_VGAResolutionRatio,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxGeneralVGAResolutionRatio(int)));
  QObject::connect(ui->checkBox_time_autoGetTime,SIGNAL(stateChanged(int)),this,SLOT(checkBoxTimeAutoGetTime(int)));
  QObject::connect(ui->comboBox_time_timeFormat,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxTimeTimeFormat(int)));
  QObject::connect(ui->comboBox_time_dateSet,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxTimeDateSet(int)));
  QObject::connect(ui->comboBox_time_timeSet,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxTimeTimeSet(int)));
  QObject::connect(ui->comboBox_time_timeArea,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxTimeTimeArea(int)));
  QObject::connect(ui->pushButton_general_sure,SIGNAL(clicked()),this,SLOT(pushButtonGeneralSure()));
  QObject::connect(ui->pushButton_general_cancel,SIGNAL(clicked()),this,SLOT(pushButtonGeneralCancel()));
  QObject::connect(ui->pushButton_general_use,SIGNAL(clicked()),this,SLOT(pushButtonGeneralUse()));

  ui->lineEdit_weihu_restart->installEventFilter(this);
  ui->lineEdit_log_selectAll->installEventFilter(this);
  ui->lineEdit_log_date->installEventFilter(this);
  ui->lineEdit_picManager_open->installEventFilter(this);
  ui->lineEdit_picManager_end->installEventFilter(this);
  ui->lineEdit_email_serverPort->installEventFilter(this);
  ui->lineEdit_email_userName->installEventFilter(this);
  ui->lineEdit_email_userpassword->installEventFilter(this);
  ui->lineEdit_email_interval->installEventFilter(this);
  ui->lineEdit_user_autoLoginOut->installEventFilter(this);
  ui->lineEdit_display_date->installEventFilter(this);
  ui->lineEdit_display_time->installEventFilter(this);
  ui->lineEdit_service_webPort->installEventFilter(this);
  ui->lineEdit_service_videoForwardingAddr->installEventFilter(this);
  ui->lineEdit_service_videoForwardingPort->installEventFilter(this);
  ui->lineEdit_service_videoUploadPort->installEventFilter(this);
  ui->lineEdit_service_videoUploadAddr->installEventFilter(this);
  ui->lineEdit_service_talkPort->installEventFilter(this);
  ui->lineEdit_wifi_ADSLName->installEventFilter(this);
  ui->lineEdit_wifi_ADSLPassword->installEventFilter(this);
  ui->lineEdit_wifi_cardPassword->installEventFilter(this);
  ui->lineEdit_wifi_cardName->installEventFilter(this);
  ui->lineEdit_wireless_ip->installEventFilter(this);
  ui->lineEdit_wireless_gateway->installEventFilter(this);
  ui->lineEdit_wireless_dns->installEventFilter(this);
  ui->lineEdit_wireless_mask->installEventFilter(this);
  ui->lineEdit_wireless_mac->installEventFilter(this);
  loadQssFile("qss/SecondDialog.qss");

}
void dialog_null_systemSetDialog::loadQssFile(QString fileName)
{
  QFile qssFile(fileName);
  QString qss;
  qssFile.open(QFile::ReadOnly);
  if(qssFile.isOpen()){
      qss = QLatin1String(qssFile.readAll());
      debug("%s\n",qss.toStdString().c_str());
      this->setStyleSheet(qss);
      qssFile.close();
    }
}
void dialog_null_systemSetDialog::testFile()
{

  QObjectList list=this->children();

  for(int a=0;a<list.size();++a){
      //     debug("--------> %s\n",list.at(a)->objectName().toStdString().c_str());
      createFunctionAndConnection(list.at(a)->objectName());
      QObjectList list0=list.at(a)->children();
      for(int b=0;b<list0.size();++b){
          //    debug(" %s\n",list0.at(b)->objectName().toStdString().c_str());
          createFunctionAndConnection(list0.at(b)->objectName());
          QObjectList list1=list0.at(b)->children();
          for(int c=0;c<list1.size();++c){
              //           debug("== %s\n",list1.at(c)->objectName().toStdString().c_str());
              createFunctionAndConnection(list1.at(c)->objectName());
              QObjectList list2=list1.at(c)->children();
              for(int d=0;d<list2.size();++d){
                  //            debug("\t %s\n",list2.at(d)->objectName().toStdString().c_str());
                  createFunctionAndConnection(list2.at(d)->objectName());
                  QObjectList list3=list2.at(d)->children();
                  for(int e=0;e<list3.size();++e){
                      //              debug("\t\t %s\n",list3.at(e)->objectName().toStdString().c_str());
                      createFunctionAndConnection(list3.at(e)->objectName());
                      QObjectList list4=list3.at(e)->children();
                      for(int f=0;f<list4.size();++f){
                          //                 debug("\t\t\t %s\n",list4.at(f)->objectName().toStdString().c_str());
                          createFunctionAndConnection(list4.at(f)->objectName());
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
  functionName.append(stringList.at(0));

  QString fatherName=stringList.at(1);
  QString selfName=stringList.at(2);

  QChar fUp=stringList.at(1).at(0).toUpper();
  QChar sUp=stringList.at(2).at(0).toUpper();

  fatherName.replace(0,1,fUp);
  selfName.replace(0,1,sUp);

  functionName.append(fatherName);
  functionName.append(selfName);

  if(stringList.at(0)=="comboBox"){

      QString functionDeclaration;
      functionDeclaration.append("void ");
      functionDeclaration.append(functionName);
      functionDeclaration.append("(int);");

      QString functionDefinition;
      functionDefinition.append("void dialog_null_systemSetDialog::");
      functionDefinition.append(functionName);
      functionDefinition.append("(int index)");
      functionDefinition.append("\n{\n\n}\n");

      /*创建连接关系*/
      QString connection;
      connection.append("QObject::connect(ui->");
      connection.append(objectName.toStdString().c_str());
      connection.append(",SIGNAL(currentIndexChanged(int)),this,SLOT(");
      connection.append(functionName.toStdString().c_str());
      connection.append("(int)));");

      // debug("%s\n",functionDeclaration.toStdString().c_str());
      //debug("%s\n",functionDefinition.toStdString().c_str());
      //  debug("%s\n",connection.toStdString().c_str());
      //  connection+=""+"("+"ui->"+objectName+","+"SIGNAL(currentIndexChanged(int))"+","+"this"+"SLOT("+objectName+")"+")";
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
      functionDefinition.append("\n{\n\n}\n");

      /*创建连接关系*/
      QString connection;
      connection.append("QObject::connect(ui->");
      connection.append(objectName.toStdString().c_str());
      connection.append(",SIGNAL(clicked()),this,SLOT(");
      connection.append(functionName.toStdString().c_str());
      connection.append("()));");

      //  debug("%s\n",functionDeclaration.toStdString().c_str());
      //  debug("%s\n",functionDefinition.toStdString().c_str());
      //  debug("%s\n",connection.toStdString().c_str());
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
      functionDefinition.append("\n{\n\n}\n");

      /*创建连接关系*/
      QString connection;
      connection.append("QObject::connect(ui->");
      connection.append(objectName.toStdString().c_str());
      connection.append(",SIGNAL(stateChanged(int)),this,SLOT(");
      connection.append(functionName.toStdString().c_str());
      connection.append("(int)));");
      // debug("%s\n",functionDeclaration.toStdString().c_str());
      //  debug("%s\n",functionDefinition.toStdString().c_str());
      //  debug("%s\n",connection.toStdString().c_str());
    }
  if(stringList.at(0)=="lineEdit"){
      QString filter;
      filter.append("ui->");
      filter.append(objectName);
      filter.append("->installEventFilter(this);");
      debug("%s\n",filter.toStdString().c_str());
      //  debug("object->objectName== \"%s\" || \\",objectName.toStdString().c_str());
    }
}
void dialog_null_systemSetDialog::pushButtonOtherYuntai()
{

}

void dialog_null_systemSetDialog::pushButtonOtherOtherDevice()
{

}

void dialog_null_systemSetDialog::pushButtonOtherMemory()
{

}

void dialog_null_systemSetDialog::comboBoxYuntaiChannelId(int index)
{

}

void dialog_null_systemSetDialog::comboBoxYuntaiBaudRate(int index)
{

}

void dialog_null_systemSetDialog::comboBoxYuntaiProtocol(int index)
{

}

void dialog_null_systemSetDialog::comboBoxYuntaiAddr(int index)
{

}

void dialog_null_systemSetDialog::checkBoxYuntaiLeftRightExchange(int flags)
{

}

void dialog_null_systemSetDialog::checkBoxYuntaiUpDownExchange(int flags)
{

}

void dialog_null_systemSetDialog::checkBoxYuntaiStopExchange(int flags)
{

}

void dialog_null_systemSetDialog::checkBoxYuntaiZoomedExchange(int flags)
{

}

void dialog_null_systemSetDialog::checkBoxYuntaiZoomExchange(int flags)
{

}

void dialog_null_systemSetDialog::comboBoxMemoryDiskSize(int index)
{

}

void dialog_null_systemSetDialog::pushButtonOtherUse()
{

}

void dialog_null_systemSetDialog::pushButtonOtherSure()
{

}

void dialog_null_systemSetDialog::pushButtonOtherCancel()
{

}

void dialog_null_systemSetDialog::pushButtonSystemInfo()
{

}

void dialog_null_systemSetDialog::pushButtonSystemLog()
{

}

void dialog_null_systemSetDialog::pushButtonSystemWeihu()
{

}

void dialog_null_systemSetDialog::checkBoxWeihuRestart(int flags)
{

}

void dialog_null_systemSetDialog::comboBoxWeihuUpdateType(int index)
{

}

void dialog_null_systemSetDialog::comboBoxWeihuServer(int index)
{

}

void dialog_null_systemSetDialog::pushButtonWeihuImportConfig()
{

}

void dialog_null_systemSetDialog::pushButtonWeihuExportConfig()
{

}

void dialog_null_systemSetDialog::pushButtonWeihuUpdate()
{

}

void dialog_null_systemSetDialog::pushButtonWeihuRegister()
{

}

void dialog_null_systemSetDialog::pushButtonWeihuFormatAll()
{

}

void dialog_null_systemSetDialog::comboBoxLogType(int index)
{

}

void dialog_null_systemSetDialog::checkBoxLogDate(int flags)
{

}

void dialog_null_systemSetDialog::pushButtonLogBackupToU()
{

}

void dialog_null_systemSetDialog::pushButtonSystemSure()
{

}

void dialog_null_systemSetDialog::pushButtonSystemUse()
{

}

void dialog_null_systemSetDialog::pushButtonSystemCancel()
{

}

void dialog_null_systemSetDialog::pushButtonPicManagerPicManager()
{

}

void dialog_null_systemSetDialog::comboBoxPicManagerFindType(int index)
{

}

void dialog_null_systemSetDialog::comboBoxPicManagerChannelId(int index)
{

}

void dialog_null_systemSetDialog::pushButtonPicManagerSelectAll()
{

}

void dialog_null_systemSetDialog::pushButtonPicManagerDel()
{

}

void dialog_null_systemSetDialog::pushButtonPicManagerBackupToU()
{

}

void dialog_null_systemSetDialog::pushButtonPicManagerFind()
{

}

void dialog_null_systemSetDialog::pushButtonPicManagerSure()
{

}

void dialog_null_systemSetDialog::pushButtonPicManagerUse()
{

}

void dialog_null_systemSetDialog::pushButtonPicManagerCancel()
{

}

void dialog_null_systemSetDialog::pushButtonWarnDeviceWarn()
{

}

void dialog_null_systemSetDialog::pushButtonWarnVideoLost()
{

}

void dialog_null_systemSetDialog::pushButtonWarnMoveTest()
{

}

void dialog_null_systemSetDialog::pushButtonWarnEmail()
{

}

void dialog_null_systemSetDialog::pushButtonVideoLostSelectAll()
{

}

void dialog_null_systemSetDialog::pushButtonVideoLostDefault()
{

}

void dialog_null_systemSetDialog::pushButtonMoveTestSelectAll()
{

}

void dialog_null_systemSetDialog::pushButtonMoveTestDefault()
{

}

void dialog_null_systemSetDialog::pushButtonEmailAddList()
{

}

void dialog_null_systemSetDialog::pushButtonEmailEditList()
{

}

void dialog_null_systemSetDialog::pushButtonEmailDelList()
{

}

void dialog_null_systemSetDialog::pushButtonEmailServerPort()
{

}

void dialog_null_systemSetDialog::comboBoxEmailServer(int index)
{

}

void dialog_null_systemSetDialog::comboBoxEmailSafeType(int index)
{

}

void dialog_null_systemSetDialog::pushButtonEmailTest()
{

}

void dialog_null_systemSetDialog::pushButtonDeviceWarnSelectAll()
{

}

void dialog_null_systemSetDialog::pushButtonDeviceWarnDefault()
{

}

void dialog_null_systemSetDialog::pushButtonWarnSure()
{

}

void dialog_null_systemSetDialog::pushButtonWarnUse()
{

}

void dialog_null_systemSetDialog::pushButtonWarnCancel()
{

}

void dialog_null_systemSetDialog::pushButtonNetworkNetservice()
{

}

void dialog_null_systemSetDialog::pushButtonNetworkWifi()
{

}

void dialog_null_systemSetDialog::pushButtonNetworkWireless()
{

}

void dialog_null_systemSetDialog::checkBoxServiceWeb(int flags)
{

}

void dialog_null_systemSetDialog::checkBoxServiceTalk(int flags)
{

}

void dialog_null_systemSetDialog::checkBoxServiceStreamServer(int flags)
{

}

void dialog_null_systemSetDialog::pushButtonWifiADSLConnect()
{

}

void dialog_null_systemSetDialog::pushButtonWifiADSLDisconnect()
{

}

void dialog_null_systemSetDialog::pushButtonWifiCardDisconnect()
{

}

void dialog_null_systemSetDialog::pushButtonWifiCardConnect()
{

}

void dialog_null_systemSetDialog::checkBoxWirelessDhcp(int flags)
{

}

void dialog_null_systemSetDialog::pushButtonNetworkCancel()
{

}

void dialog_null_systemSetDialog::pushButtonNetworkSure()
{

}

void dialog_null_systemSetDialog::pushButtonNetworkUse()
{

}

void dialog_null_systemSetDialog::pushButtonChannelStream()
{

}

void dialog_null_systemSetDialog::pushButtonChannelAudio()
{

}

void dialog_null_systemSetDialog::pushButtonChannelVideo()
{

}

void dialog_null_systemSetDialog::pushButtonChannelPreview()
{

}

void dialog_null_systemSetDialog::pushButtonVideoSetVideoCopy()
{

}

void dialog_null_systemSetDialog::pushButtonVideoSetDefault()
{

}

void dialog_null_systemSetDialog::pushButtonPreviewCopy()
{

}

void dialog_null_systemSetDialog::pushButtonPreviewSetDefault()
{

}

void dialog_null_systemSetDialog::pushButtonPreviewChannelSort()
{

}

void dialog_null_systemSetDialog::pushButtonChannelSure()
{

}

void dialog_null_systemSetDialog::pushButtonChannnelUse()
{

}

void dialog_null_systemSetDialog::pushButtonChannelCancel()
{

}

void dialog_null_systemSetDialog::pushButtonGeneralGeneral()
{

}

void dialog_null_systemSetDialog::pushButtonGeneralTime()
{

}

void dialog_null_systemSetDialog::pushButtonGeneralDisplay()
{

}

void dialog_null_systemSetDialog::pushButtonGeneralUser()
{

}

void dialog_null_systemSetDialog::comboBoxUserUserGroup(int index)
{

}

void dialog_null_systemSetDialog::comboBoxUserUserList(int index)
{

}

void dialog_null_systemSetDialog::pushButtonUserUserAdd()
{

}

void dialog_null_systemSetDialog::pushButtonUserUserEdit()
{

}

void dialog_null_systemSetDialog::pushButtonUserUserDel()
{

}

void dialog_null_systemSetDialog::checkBoxDisplaySensorData(int flags)
{

}

void dialog_null_systemSetDialog::checkBoxGeneralGuide(int flags)
{

}

void dialog_null_systemSetDialog::comboBoxGeneralLanguage(int index)
{

}

void dialog_null_systemSetDialog::comboBoxGeneralVideoFormat(int index)
{

}

void dialog_null_systemSetDialog::comboBoxGeneralVideoResolutionRatio(int index)
{

}

void dialog_null_systemSetDialog::comboBoxGeneralTransparent(int index)
{

}

void dialog_null_systemSetDialog::comboBoxGeneralVGAResolutionRatio(int index)
{

}

void dialog_null_systemSetDialog::checkBoxTimeAutoGetTime(int flags)
{

}

void dialog_null_systemSetDialog::comboBoxTimeTimeFormat(int index)
{

}

void dialog_null_systemSetDialog::comboBoxTimeDateSet(int index)
{

}

void dialog_null_systemSetDialog::comboBoxTimeTimeSet(int index)
{

}

void dialog_null_systemSetDialog::comboBoxTimeTimeArea(int index)
{

}

void dialog_null_systemSetDialog::pushButtonGeneralSure()
{

}

void dialog_null_systemSetDialog::pushButtonGeneralCancel()
{

}

void dialog_null_systemSetDialog::pushButtonGeneralUse()
{

}

