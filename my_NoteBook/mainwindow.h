#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include     "scribblearea.h"

class BackGround;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public:
    Ui::MainWindow *getUI(void){return ui;}
private:
    void initEvent(void);

private slots:
    /* Help menu*/
     void about();
public slots:
    /* File menu */
    void myOpenFile();
    void mySaveFile();
    void mySaveAsFile();
    void myCreateNewFile();
    void myPrint();

    /* Edit menu */
    void myRevoke();
    void myCut();
    void myCopy();
    void myPaste();
    void myDelete();
    void myFind();
    void myFindNext();
    void myReplace();
    void myGoto();
    void mySelectAll();
    void myDatatime();
    void myAboutFile();

    /*Format menu*/
    void myAutoWrap();
    void myFront();

    /* Check menu */
    void myStatus();
private:
    Ui::MainWindow *ui;
    BackGround *bg;
    ScribbleArea *scribbleArea;
};

#endif // MAINWINDOW_H
