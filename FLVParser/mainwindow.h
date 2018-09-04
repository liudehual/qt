#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "fvlparserthread.h"
#include <QStandardItem>
#include "flvtag.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void sendFileName(QString fileName);
public slots:
    void OpenFile();
    void Exit();
    void AboutFLVParser();
    void AboutQt();

    void UpdateListView();
    void ParseSpecialTag(QModelIndex i);
private:
    void ParseAudio(unsigned char first,FLVTag *tTag);
    void ParseVideo(unsigned char first,FLVTag *tTag);
    void ParseScript(unsigned char first,FLVTag *tTag);
private:
    Ui::MainWindow *ui;
    FVLParserThread *flvThread;


};

#endif // MAINWINDOW_H
