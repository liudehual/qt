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
private:
    Ui::MainWindow *ui;
    BackGround *bg;
    ScribbleArea *scribbleArea;
};

#endif // MAINWINDOW_H
