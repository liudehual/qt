#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
  class dialog_null_systemSetDialog;
}
class dialog_null_TipDialog;
class dialog_null_systemSetDialog : public QDialog
{
  Q_OBJECT

public:
  explicit dialog_null_systemSetDialog(QWidget *parent = 0);
  ~dialog_null_systemSetDialog();

  int getContentSize(){return content.size();}
signals:
public slots:
  void pushButtonSystemInfo();
  void pushButtonSystemLog();
  void pushButtonSystemWeihu();
  void checkBoxWeihuRestart(int);
  void comboBoxWeihuUpdateType(int);
  void comboBoxWeihuServer(int);
  void pushButtonWeihuImportConfig();
  void pushButtonWeihuExportConfig();
  void pushButtonWeihuUpdate();
  void pushButtonWeihuRegister();
  void pushButtonWeihuFormatAll();
  void comboBoxLogType(int);
  void checkBoxLogDate(int);
  void pushButtonLogBackupToU();
  void pushButtonSystemSure();
  void pushButtonSystemUse();
  void pushButtonSystemCancel();
  void pushButtonPicManagerPicManager();
  void comboBoxPicManagerFindType(int);
  void comboBoxPicManagerChannelId(int);
  void pushButtonPicManagerSelectAll();
  void pushButtonPicManagerDel();
  void pushButtonPicManagerBackupToU();
  void pushButtonPicManagerFind();
  void pushButtonPicManagerSure();
  void pushButtonPicManagerUse();
  void pushButtonPicManagerCancel();
  void pushButtonWarnDeviceWarn();
  void pushButtonWarnVideoLost();
  void pushButtonWarnMoveTest();
  void pushButtonWarnEmail();
  void pushButtonVideoLostSelectAll();
  void pushButtonVideoLostDefault();
  void pushButtonMoveTestSelectAll();
  void pushButtonMoveTestDefault();
  void pushButtonEmailAddList();
  void pushButtonEmailEditList();
  void pushButtonEmailDelList();
  void pushButtonEmailServerPort();
  void comboBoxEmailServer(int);
  void comboBoxEmailSafeType(int);
  void pushButtonEmailTest();
  void pushButtonDeviceWarnSelectAll();
  void pushButtonDeviceWarnDefault();
  void pushButtonWarnSure();
  void pushButtonWarnUse();
  void pushButtonWarnCancel();
  void pushButtonNetworkNetservice();
  void pushButtonNetworkWifi();
  void pushButtonNetworkWireless();
  void checkBoxServiceWeb(int);
  void checkBoxServiceTalk(int);
  void checkBoxServiceStreamServer(int);
  void pushButtonWifiADSLConnect();
  void pushButtonWifiADSLDisconnect();
  void pushButtonWifiCardDisconnect();
  void pushButtonWifiCardConnect();
  void checkBoxWirelessDhcp(int);
  void pushButtonNetworkCancel();
  void pushButtonNetworkSure();
  void pushButtonNetworkUse();
  void pushButtonChannelStream();
  void pushButtonChannelAudio();
  void pushButtonChannelVideo();
  void pushButtonChannelPreview();
  void pushButtonVideoSetVideoCopy();
  void pushButtonVideoSetDefault();
  void pushButtonPreviewCopy();
  void pushButtonPreviewSetDefault();
  void pushButtonPreviewChannelSort();
  void pushButtonChannelSure();
  void pushButtonChannnelUse();
  void pushButtonChannelCancel();
  void pushButtonGeneralGeneral();
  void pushButtonGeneralTime();
  void pushButtonGeneralDisplay();
  void pushButtonGeneralUser();
  void comboBoxUserUserGroup(int);
  void comboBoxUserUserList(int);
  void pushButtonUserUserAdd();
  void pushButtonUserUserEdit();
  void pushButtonUserUserDel();
  void checkBoxDisplaySensorData(int);
  void checkBoxGeneralGuide(int);
  void comboBoxGeneralLanguage(int);
  void comboBoxGeneralVideoFormat(int);
  void comboBoxGeneralVideoResolutionRatio(int);
  void comboBoxGeneralTransparent(int);
  void comboBoxGeneralVGAResolutionRatio(int);
  void checkBoxTimeAutoGetTime(int);
  void comboBoxTimeTimeFormat(int);
  void comboBoxTimeDateSet(int);
  void comboBoxTimeTimeSet(int);
  void comboBoxTimeTimeArea(int);
  void pushButtonGeneralSure();
  void pushButtonGeneralCancel();
  void pushButtonGeneralUse();
  void pushButtonOtherYuntai();
  void pushButtonOtherOtherDevice();
  void pushButtonOtherMemory();
  void comboBoxYuntaiChannelId(int);
  void comboBoxYuntaiBaudRate(int);
  void comboBoxYuntaiProtocol(int);
  void comboBoxYuntaiAddr(int);
  void checkBoxYuntaiLeftRightExchange(int);
  void checkBoxYuntaiUpDownExchange(int);
  void checkBoxYuntaiStopExchange(int);
  void checkBoxYuntaiZoomedExchange(int);
  void checkBoxYuntaiZoomExchange(int);
  void comboBoxMemoryDiskSize(int);
  void pushButtonOtherUse();
  void pushButtonOtherSure();
  void pushButtonOtherCancel();

protected:
  bool eventFilter(QObject *, QEvent *);


private:
  void initWidget();
  void loadQssFile(QString fileName);
private:
  void testFile();
  void createFunctionAndConnection(QString);
private:
  Ui::dialog_null_systemSetDialog *ui;
  QString content;
  dialog_null_TipDialog *tDialog;
};

#endif // DIALOG_H
